#include <napi.h>       // Заголовочный файл для использования node-addon-api
#include <windows.h>    // Заголовочный файл для функций Windows API
#include <lm.h>         // Заголовочный файл для функций работы с учетными записями
#include <string>       // Заголовочный файл для работы со строками
#pragma comment(lib, "netapi32.lib")  // Директива для автоматической ссылки на библиотеку netapi32.lib

// Функция для получения привилегий пользователя
std::string GetUserPrivilege(const std::string& username) {
    USER_INFO_3* userInfo = NULL;
    // Преобразование имени пользователя в формат wchar_t для использования с WinAPI
    // Вызов WinAPI функции для получения информации о пользователе
    NET_API_STATUS status = NetUserGetInfo(NULL, std::wstring(username.begin(), username.end()).c_str(), 3, (LPBYTE*)&userInfo);

    // Проверка успешности вызова NetUserGetInfo
    if (status != NERR_Success) {
        return "Пользователя " + username + " нет.";   // Возвращаем "нет", если не удалось получить информацию о пользователе
    }

    std::string privilege;
    // Определение привилегии пользователя на основе полученной информации
    if (userInfo->usri3_priv == USER_PRIV_ADMIN) {
        privilege = "Администратор";
    } else if (userInfo->usri3_priv == USER_PRIV_USER) {
        privilege = "Пользователь";
    } else if (userInfo->usri3_priv == USER_PRIV_GUEST) {
        privilege = "Гость";
    } else {
        privilege = "Неизвестно";
    }
    // Освобождение выделенной памяти для структуры USER_INFO_3
    NetApiBufferFree(userInfo);
    return "Пользователь " + username + " имеет привилегию " + privilege;  // Возвращаем строку с привилегией пользователя
}

// Функция-обертка для проверки привилегий пользователя через node-addon-api
Napi::String checkPrivileges(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Получение имени пользователя из аргумента
    std::string username = info[0].As<Napi::String>().Utf8Value();
    
    // Вызов функции GetUserPrivilege для получения привилегий пользователя
    std::string privilege = GetUserPrivilege(username);
    
    // Возвращение результата в виде Napi::String
    return Napi::String::New(env, privilege);
}

// Инициализация модуля и экспорт функции checkPrivileges в node-addon-api
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "checkPrivileges"), Napi::Function::New(env, checkPrivileges));
    return exports;
}

// Макрос для создания модуля node-addon-api с именем addon и инициализационной функцией Init
NODE_API_MODULE(addon, Init)
