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

mkdir -p ${buildDir}

emcmake cmake -B ${buildDir} -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten "-DCMAKE_EXE_LINKER_FLAGS=-s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=[ccall] -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=[_main,_malloc,_free] --preload-file ../../Chip8Games --preload-file ../../shaders --preload-file ../../fonts" -DCMAKE_BUILD_TYPE=Release

emmake make -C ${buildDir}
