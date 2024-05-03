#!/bin/bash
if [ -d "build" ]; then
    rm -rf build
fi
make -j4
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi
sh upload.sh