#!/bin/bash
make -j4
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi
sh upload.sh