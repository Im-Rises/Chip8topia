#!/bin/bash

if [ -z "$EMSDK" ]; then
    echo "EMSDK is not set"
    exit 1
fi

# Set buildDir if not argument is provided
if [ -z "$1" ]; then
    buildDir="buildWasm"
else
    buildDir=$1
fi

projectName="Chip8topia"

rm -rf ${buildDir}

mkdir -p ${buildDir}

emcmake cmake -B ${buildDir} -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten

emmake make -C ${buildDir}

cp ./web/* ${buildDir}/${projectName}

python3 -m http.server -d ${buildDir}/${projectName}

exit 0