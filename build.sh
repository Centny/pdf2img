#!/bin/bash
set -e
#clang-format -style=file -i -sort-includes **/*.cpp **/*.hpp #**/*.hpp **/*.cc **/*.c
# export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:.
if [ "$1" = "test" -o "$1" = "testc" ];then
    if [ "$1" = "testc" ];then
        rm -rf build
    fi
    export GYP_DEFINES="other_cflags='-fprofile-arcs -ftest-coverage' other_lflags='-fprofile-arcs -ftest-coverage'"
    gyp --depth=. --build=Default  --generator-output=build
    #xcodebuild -project build/pdf2im.xcodeproj -configuration Default
else
    gyp --depth=.
    xcodebuild -project pdf2img.xcodeproj -configuration Default
fi
# rm -rf build

if [ "$1" = "test" -o "$1" = "testc" ];then
    cd ./build/Default/
    if [ "$2" = "" ];then
        ./pdf2img.test 2>err.log
    else
        ./pdf2img.test --run_test=$2 2>err.log
    fi
    cd ../../
    echo "All test done..."
    rm -rf report
    mkdir -p report/html
    gcovr --html --html-details -o report/html/coverage.html -r . -v build/pdf2img.build/Default/pdf2img.build/ 1>build/c.log
    echo "Build converage report done..."
fi

if [ "$1" = "run" ];then
    cd ./build/Default/
    ./pdf2img_cli
    cd ../../
fi

echo "All done..."