{
  "targets": [
    {
      "target_name": "usnit",
      "type": "static_library",
      "dependencies": [
        "gearsbox/gearsbox.gyp:gearsbox"
      ],
      "sources": [
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./generated-src/cpp   '*.cpp' '*.hpp' '*.h')",
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./cpp '*.cpp' '*.hpp' '*.c' '*.h')",
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./interface '*.hpp' '*.h')",
      ],
      "include_dirs": [
        "generated-src/cpp",
        "cpp",
        'gearsbox/src/cpp/ui',
      ],
    },
    {
    "target_name": "usnit_objc",
      "type": "static_library",
      "dependencies": [
        "usnit",
        "gearsbox/gearsbox.gyp:gearsbox_objc"
      ],
      "sources": [
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./generated-src/objc  '*.cpp' '*.h' '*.mm' '*.m')",
      ],
      "include_dirs": [
        "generated-src/objc",
        "generated-src/cpp",
        "cpp",
        "gearsbox/deps/djinni/support-lib/objc",
        'gearsbox/src/cpp/ui',
      ],
    },
    {
      "target_name": "usnit_jni",
      "type": "static_library",
      "dependencies": [
        "usnit",
        "./gearsbox/gearsbox.gyp:gearsbox_jni"
      ],
      "ldflags" : [ "-llog", "-Wl,--build-id,--gc-sections,--exclude-libs,ALL" ],
      "sources": [
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./generated-src/jni   '*.cpp')",
      ],
      "include_dirs": [
        "generated-src/jni",
        "generated-src/cpp",
        "cpp",
        "./gearsbox/deps/djinni/support-lib/jni",
      ],
    },
    {
      'target_name': 'test',
      'type': 'executable',
      'dependencies': [
        'usnit',
        "gearsbox/gearsbox.gyp:test",
        'gearsbox/deps/gtest.gyp:gtest',
      ],
      'cflags_cc!': [ '-Werror', '-Wextra' ],
      'xcode_settings': {
        'OTHER_CPLUSPLUSFLAGS!' : ['-Werror', '-Wextra'],
      },
      'include_dirs': [
        '.',
        'test',
      ],
      "defines": [
        'TEST_STAND_ALONE',
        'GEARSBOX_TEST',
      ],
      'sources': [
        '<!@(python gearsbox/deps/djinni/example/glob.py test *.cpp *.hpp *.h)',
      ]
    },
  ],
}