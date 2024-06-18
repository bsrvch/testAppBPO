// Импортируем необходимые модули и функции из библиотеки Express
import express from 'express';
import path from 'path';
import { fileURLToPath } from 'url'; // Модуль для преобразования URL в путь к файлу
import { dirname } from 'path'; // Модуль для получения имени директории

// Определение текущего файла и директории с использованием import.meta.url
const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

// Импорт нативного модуля Node.js, который связывает Node.js и C++ модули
import addon from 'bindings';

// Создание экземпляра приложения Express
const app = express();

// Порт, на котором будет работать сервер Express
const port = 3000;

// Middleware для обработки данных из формы
app.use(express.urlencoded({ extended: true }));

// Middleware для обслуживания статических файлов из директории 'public'
app.use(express.static(__dirname + '/public'));

// Маршрут для обработки GET запроса на главной странице
app.get('/', (req, res) => {
    // Отправляем файл index.html из папки 'public'
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Маршрут для обработки POST запроса на проверку привилегий пользователя
app.post('/check', (req, res) => {
    // Извлекаем имя пользователя из POST запроса
    const username = req.body.username;
    
    // Вызываем функцию checkPrivileges из нашего C++ модуля addon
    const privilege = addon(`addon`)['checkPrivileges'](username);

    // Отправляем HTML страницу с результатом проверки привилегий
    res.send(`
    <html>
      <link rel="stylesheet" type="text/css" href="./style.css" media="screen" />
      <body>
       <div class="container">
         <p>${privilege}</p>
         <button class="submit-btn" onclick="window.location.href='/'">Назад</button>
       </div>
      </body>
    </html>
  `);
});

// Запуск сервера Express на заданном порту
app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}`);
});
