{
    "variables": {
        "other_cflags%": "",
        "other_lflags%": "",
        "deps%": [
            "-lpdfium",
            "-ljpeg",
        ],
    },
    'targets': [
        {
            "variables": {
                "other_cflags%": "",
                "other_lflags%": "",
            },
            'target_name': 'pdf2img',
            'type': 'static_library',
            'include_dirs': [
                '/usr/local/include',
                '/usr/include',
                "./deps/precompiled/include",
            ],
            'dependencies': [
            ],
            'sources': [
                'src/pdf2img.cc',
                'src/pdf2img.h',
            ],
            'conditions': [
                ['OS=="mac"', {
                    'xcode_settings': {
                        'MACOSX_DEPLOYMENT_TARGET': '10.11',
                        'LD_DYLIB_INSTALL_NAME': "@rpath/$(EXECUTABLE_PATH)",
                        'OTHER_CFLAGS': [
                            "<(other_cflags)",
                        ],
                        'OTHER_LDFLAGS': [
                            "-stdlib=libc++",
                            "-framework CoreFoundation",
                            "-framework CoreGraphics",
                            "-L/usr/local/lib",
                            "-L./deps/precompiled/lib",
                            "-Wl,-rpath,.",
                            "<(other_lflags)",
                            "<(deps)",
                        ],
                    },
                }],
                ['OS=="linux"', {
                    'ldflags': [
                        '-pthread',
                        "-L.",
                        "-L/usr/local/lib",
                        "-Wl,-rpath,.",
                    ],
                    'cflags': [
                        '-fPIC',
                    ],
                }],
            ],
        },
        {
            'target_name': 'pdf2img.test',
            'type': 'executable',
            'include_dirs': [
                '/usr/local/include',
                '/usr/include',
                "./deps/precompiled/include",
            ],
            'sources': [
                'src/test_main.cc',
                'src/pdf2img_test.cc',
            ],
            'dependencies': [
                'pdf2img',
            ],
            'conditions': [
                ['OS=="mac"', {
                    'xcode_settings': {
                        'MACOSX_DEPLOYMENT_TARGET': '10.11',
                        'LD_DYLIB_INSTALL_NAME': "@rpath/$(EXECUTABLE_PATH)",
                        'OTHER_CFLAGS': [
                            "<(other_cflags)",
                        ],
                        'OTHER_LDFLAGS': [
                            "-stdlib=libc++",
                            "-lgtest",
                            "-framework CoreFoundation",
                            "-framework CoreGraphics",
                            "-L/usr/local/lib",
                            "-L.",
                            "-L./deps/precompiled/lib",
                            "-Wl,-rpath,.",
                            "<(other_lflags)",
                            "<(deps)",
                        ],
                    },
                }],
                ['OS=="linux"', {
                    'ldflags': [
                        '-pthread',
                        "-L.",
                        "-L/usr/local/lib",
                        "-Wl,-rpath,.",
                    ],
                    'cflags': [
                        '-fPIC',
                    ],
                }],
            ],
        },
        {
            'target_name': 'pdf2img_cli',
            'type': 'executable',
            'include_dirs': [
                '/usr/local/include',
                '/usr/include',
                "./deps/precompiled/include",
            ],
            'dependencies': [
                # 'pdf2img',
            ],
            'sources': [
                'src/pdf2img_cli.cc',
            ],
            "ldflags":[
                "<(deps)",
            ],
            'cflags': [
            ],
            'conditions': [
                ['OS=="mac"', {
                    'xcode_settings': {
                        'MACOSX_DEPLOYMENT_TARGET': '10.11',
                        'LD_DYLIB_INSTALL_NAME': "@rpath/$(EXECUTABLE_PATH)",
                        'OTHER_CFLAGS': [
                            "<(other_cflags)",
                        ],
                        'OTHER_LDFLAGS': [
                            "-stdlib=libc++",
                            "-framework CoreFoundation",
                            "-framework CoreGraphics",
                            "-L/usr/local/lib",
                            "-L.",
                            "-L./deps/precompiled/lib",
                            "-Wl,-rpath,.",
                            "<(other_lflags)",
                            "<(deps)",
                        ],
                    },
                }],
                ['OS=="linux"', {
                    'ldflags': [
                        '-pthread',
                        "-L.",
                        "-L/usr/local/lib",
                        "-Wl,-rpath,.",
                    ],
                    'cflags': [
                        '-fPIC',
                    ],
                }],
            ],
        },
    ],
}
