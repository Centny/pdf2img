#!/bin/bash
set -e
./build.sh
./build/Default/pdf2img.test $1