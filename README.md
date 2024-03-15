# Chip8topia

<p align="center">
      <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cppLogo" style="height:60px;"/>
</p>

## Description

Chip8topia is a Chip8 emulator written in C++ for Windows, Linux and WebAssembly. It uses OpenGL/WebGL for the rendering
and ImGui for the UI.

It can emulate the original Chip8, the SCHIP1.1 and the SCHIPC. I will add the Xo-Chip in the future.

## Screenshots

## Documentation

### Main resources

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Dxyn
https://chip8.gulrak.net/#quirk4
https://github.com/Timendus/chip8-test-suite
https://chip-8.github.io/extensions/#chip-48
https://tobiasvl.github.io/blog/write-a-chip-8-emulator/

### Other resources

https://en.wikipedia.org/wiki/CHIP-8#Further_reading
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
http://tobiasvl.github.io/blog/write-a-chip-8-emulator/
https://www.laurencescotford.net/2020/07/19/chip-8-on-the-cosmac-vip-arithmetic-and-logic-instructions/
https://github.com/Timendus/chip8-test-suite
https://www.laurencescotford.net/2020/07/19/chip-8-on-the-cosmac-vip-drawing-sprites/
https://chip8.gulrak.net/
https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy1-binary-or
https://emscripten.org/docs/porting/files/packaging_files.html
https://webgl2fundamentals.org/webgl/lessons/webgl-data-textures.html
https://emscripten.org/docs/porting/files/index.html
https://emscripten.org/docs/porting/files/file_systems_overview.html#file-system-overview
https://emscripten.org/docs/porting/files/Synchronous-Virtual-XHR-Backed-File-System-Usage.html#synchronous-virtual-xhr-backed-file-system-usage
https://github.com/marketplace/actions/upload-release-asset
https://github.com/mymindstorm/setup-emsdk
https://github.com/Armchair-Software/emscripten-browser-file

## Dependencies

- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [stb](https://github.com/nothings/stb)
- [spdlog](https://github.com/gabime/spdlog)
- [Vcpkg](https://vcpkg.io/en)
- [ImGui](https://github.com/ocornut/imgui)
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)
- [ImGui Memory Editor](https://github.com/ocornut/imgui_club/tree/main/imgui_memory_editor)
- [emscripten-browser-file](https://github.com/Armchair-Software/emscripten-browser-file)
- [ImGuiNotify](https://github.com/TyomaVader/ImGuiNotify/tree/Dev)
- [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders)
- [Font-Awesome](https://github.com/FortAwesome/Font-Awesome)

## Build

### Submodule:

```bash
git submodule update --init --recursive
```

```bash
git submodule update --remote
```

### Vcpkg:

In order to use vcpkg with CMake outside an IDE, you can use the toolchain file:

```bash
cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

Then build with:

```bash
cmake --build [build directory]
```

### Emscripten

```bash
emcmake cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${{ github.workspace }}/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten "-DCMAKE_EXE_LINKER_FLAGS=-s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=[ccall] -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=[_main,_malloc,_free] --preload-file ../../Chip8Games --preload-file ../../shaders --preload-file ../../fonts" -DCMAKE_BUILD_TYPE=Release
```

then build with:

```bash
emmake make -C build
```

## TODO

Major:

- [-] Add Xo-Chip emulation
- [-] Add safe and unsafe code version (set the callback error in Core and Cpu, and Ppu, the call it on error)

Minor:

- [ ] Add a way to put front the windows on the main window click
- [ ] Handle the issues with AZERTY keyboard and QWERTY keyboard (Desktop and Web)
- [ ] Add a way to change the input keys
- [ ] if none of the roms are loaded, we should display a background with written Chip8
- [ ] Add audio
- [ ] Add save states ? (if yes create a window with an image preview of the save state) add in File main bar section
- [ ] Do a general optimization of the code

