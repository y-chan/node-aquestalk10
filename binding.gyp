{
  "targets": [
    {
      "target_name": "aquestalk10",
      "sources": [
        "aquestalk10.cc",
        "aquestalk10.h",
        "aquestalk10/common.h"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ],
      "conditions": [
        [
          "OS=='win'",
          {
            "sources": [
              "aquestalk10/win.cpp",
              "aquestalk10/win.h"
            ],
            "msvs_settings": {
              "VCCLCompilerTool": {
                "ExceptionHandling": "2"
              },
            }
          }
        ],
        [
          "OS=='linux'",
          {
            "sources": [
              "aquestalk10/linux.cpp",
              "aquestalk10/linux.h"
            ]
          }
        ],
        [
          "OS=='mac'",
          {
            "sources": [
              "aquestalk10/linux.cpp",
              "aquestalk10/linux.h"
            ],
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
            }
          }
        ]
      ]
    }
  ]
}
