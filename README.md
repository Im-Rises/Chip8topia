# Chip8topia

<p align="center">
      <img src="https://user-images.githubusercontent.com/59691442/183268126-b3d19e66-8f2d-463a-805e-ae6ef7cc6c01.png" alt="cmakeLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cppLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl" alt="openglLogo" style="height:60px;"/>
</p>

## Description

Chip8topia is a Chip8 emulator written in C++ for Windows, Linux and WebAssembly. It uses OpenGL/WebGL for the rendering
and ImGui for the UI.
The application is compiled with CMake and uses Vcpkg for the dependencies.
For the WebAssembly version, it uses Emscripten and vcpkg for most dependencies.

It can emulate the original Chip8, the SCHIP1.1, SCHIPC and Xo-Chip.
Currently, no sound is implemented but it is planned.

## Screenshots

## TODO

Major:

- [ ] Correct Xo-Chip Emulation and the integrated games not working for all of the chips

Minor:

- [-] Add in the cores an exit this clock when reaching an error or the EXIT opcode
- [-] Add safe and unsafe code version (set the callback error in Core and Cpu, and Ppu, the call it on error)
- [ ] Add a way to change the input keys
- [ ] Add a background when no rom is loaded
- [ ] Add audio
- [ ] Add save states ? (if yes create a window with an image preview of the save state) add in File main bar section
- [ ] Do a general optimization and cleanup of the code

## Documentation

- [devernay.free.fr](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [chip8.gulrak.net](https://chip8.gulrak.net/)
- [Timendus/chip8-test-suite](https://github.com/Timendus/chip8-test-suite)
- [chip-8.github.io](https://chip-8.github.io/)
- [tobiasvl.github.io](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
- [johnearnest.github.io](http://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html)
- [Timendus/silicon8](https://github.com/Timendus/silicon8/tree/)

<!--
### Other resources

https://en.wikipedia.org/wiki/CHIP-8
https://www.laurencescotford.net/2020/07/19/chip-8-on-the-cosmac-vip-arithmetic-and-logic-instructions/
https://www.laurencescotford.net/2020/07/19/chip-8-on-the-cosmac-vip-drawing-sprites/
https://webgl2fundamentals.org/webgl/lessons/webgl-data-textures.html
https://emscripten.org/docs/porting/files/packaging_files.html
https://emscripten.org/docs/porting/files/index.html
https://emscripten.org/docs/porting/files/file_systems_overview.html#file-system-overview
https://emscripten.org/docs/porting/files/Synchronous-Virtual-XHR-Backed-File-System-Usage.html#synchronous-virtual-xhr-backed-file-system-usage
https://github.com/marketplace/actions/upload-release-asset
https://github.com/mymindstorm/setup-emsdk
https://github.com/Armchair-Software/emscripten-browser-file
-->

## Dependencies

- [Vcpkg](https://vcpkg.io/en)
- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [stb](https://github.com/nothings/stb)
- [spdlog](https://github.com/gabime/spdlog)
- [ImGui](https://github.com/ocornut/imgui)
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)
- [ImGui Memory Editor](https://github.com/ocornut/imgui_club/tree/main/imgui_memory_editor)
- [ImGuiNotify](https://github.com/TyomaVader/ImGuiNotify/tree/Dev)
- [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders)
- [Font-Awesome](https://github.com/FortAwesome/Font-Awesome)
- [emscripten-browser-file](https://github.com/Armchair-Software/emscripten-browser-file)

## Build

Before building, be sure to fetch all the git submodules and install vcpkg.

### Submodule:

To fetch the submodules:

```bash
git submodule update --init --recursive
```

If you want to update the submodules:

```bash
git submodule update --remote
```

### Build

Once you have fetched the submodules and installed vcpkg, you can build the project.
Go to the root of the project and create a build directory.

> **Note**
> Be sure to change the path to vcpkg or emscripten if necessary in the following commands.

#### Build for Windows or Linux

Use the following command to set up the build:

```bash
cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

Then build with:

```bash
cmake --build [build directory]
```

### Build for WebAssembly (Emscripten)

Use the following command to set up the build:

```bash
emcmake cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten "-DCMAKE_EXE_LINKER_FLAGS=-s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=[ccall] -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=[_main,_malloc,_free] --preload-file ../../Chip8Games --preload-file ../../shaders --preload-file ../../fonts" -DCMAKE_BUILD_TYPE=Release
```

then build with:

```bash
emmake make -C [build directory]
```

## Contributors

Quentin MOREL :

- @Im-Rises
- <https://github.com/Im-Rises>

[![GitHub contributors](https://contrib.rocks/image?repo=Im-Rises/Chip8topia)](https://github.com/Im-Rises/Chip8topia/graphs/contributors)