{
  "targets": [
    {
      "target_name": "usnit_objc",
      "type": "static_library",
      "dependencies": [
        "gearsbox/gearsbox.gyp:gearsbox_objc"
      ],
      "sources": [
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./generated-src/objc  '*.cpp' '*.h' '*.mm' '*.m')",
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./generated-src/cpp   '*.cpp' '*.hpp' '*.h')",
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./cpp '*.cpp' '*.hpp' '*.c' '*.h')",
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./interface '*.hpp' '*.h')",
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
        "./gearsbox/gearsbox.gyp:gearsbox_jni"
      ],
      "ldflags" : [ "-llog", "-Wl,--build-id,--gc-sections,--exclude-libs,ALL" ],
      "sources": [
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./generated-src/jni   '*.cpp')",
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./generated-src/cpp   '*.cpp')",
        "<!@(python ./gearsbox/deps/djinni/example/glob.py ./cpp '*.cpp')",
      ],
      "include_dirs": [
        "generated-src/jni",
        "generated-src/cpp",
        "cpp",
        "./gearsbox/deps/djinni/support-lib/jni",
      ],
    },
  ],
}