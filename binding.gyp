{
  "targets": [
    {
      "target_name": "addon",
      "sources": ["addon/addon.cpp"],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1
        },
        "Link": {
          "AdditionalDependencies": ["Netapi32.lib"]
        }
      },
      "defines": [ "NAPI_CPP_EXCEPTIONS" ]
    }
  ]
}
