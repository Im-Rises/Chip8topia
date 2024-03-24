# Chip8topia

<p align="center">
      <img src="https://user-images.githubusercontent.com/59691442/183268126-b3d19e66-8f2d-463a-805e-ae6ef7cc6c01.png" alt="cmakeLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cppLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl" alt="openglLogo" style="height:60px;"/>
</p>

## Description

Chip8topia is a Chip8 emulator written in C++ for Windows, Linux and WebAssembly. It uses OpenGL/WebGL for the rendering
and ImGui for the UI.
The application is compiled with CMake and uses Vcpkg for most dependencies.
For the WebAssembly version, it uses Emscripten to compile with vcpkg for most dependencies as well.

It can emulate the original Chip8, the SCHIP1.1, SCHIPC and Xo-Chip.
Currently, no sound is implemented, but it is planned.

Emulatated consoles:

- [x] Chip8
- [x] SCHIP1.1
- [x] SCHIPC
- [x] Xo-Chip

## Features

- [x] Load Chip8, SCHIP1.1, SCHIPC and Xo-Chip games
- [x] Pause/Resume the emulation
- [x] Restart the emulation
- [x] Change the emulation speed
- [x] Change the color of the screen
- [x] Disassemble
- [x] Breakpoint
- [x] Memory Editor
- [x] Register Editor
- [x] Plane viewer (SCHIP1.1, SCHIPC and Xo-Chip)
- [x] Desktop and WebAssembly version
- [x] Load ROMs from the browser (WebAssembly)
- [x] Load integrated ROMs (WebAssembly)

## Screenshots

Screenshot of the differents debug tools here ?

| Invaders (Chip8)                                                                                                         | Car (SCHIP)                                                                                                         | T8NKS (Xo-Chip)                                                                                                       |                                                                                                                   
|--------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| ![invaders-game-screenshot](https://github.com/Im-Rises/Chip8topia/assets/59691442/3e5a2078-2cf7-4929-94c2-ea42cea24efd) | ![car-game-screenshot](https://github.com/Im-Rises/Chip8topia/assets/59691442/13774fed-1ce4-4ab6-b73d-40aa374661a0) | ![t8nks-game-screenshot](https://github.com/Im-Rises/Chip8topia/assets/59691442/e4ab6b1f-e5a4-485f-a26b-dab63e20aff1) |

| Alien Inv8sion | 
|----------------|
|                |

## TODO

Major:

- [ ] Correct the Xo-Chip games not working properly 5think its the CPU that is not working properlym the Ppu should be
  fine)
- [ ] Restarting a game when playing big xo-chip games will not reload the game from the start (we should reset memory
  and reload the game rom)

Future:

- [ ] Add audio
- [ ] Add safe and unsafe code version (set the callback error in Core and Cpu, and Ppu, the call it on error) Create an
  error code when calling Core.clock(); if it returns 1 it means it needs a screen refresh if 2 its an
  error and -1 means emulation issue, so the program should stop (also add a normal exit on EXIT opcode)

Minor:

- [ ] Add a way to change the input keys
- [ ] Add a background when no rom is loaded
- [ ] Check which imgui window is very slow (probably the memory editor)
- [ ] Correct the clean of the debug image plane (they are not cleared when changing game)

## Controls

### Original game controls

| 1 | 2 | 3 | C |
|---|---|---|---|
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |

### Emulator game controls (QWERTY)

| 1 | 2 | 3 | 4 |
|---|---|---|---|
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |

## Documentation

- [devernay.free.fr](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [chip8.gulrak.net](https://chip8.gulrak.net/)
- [Timendus/chip8-test-suite](https://github.com/Timendus/chip8-test-suite)
- [chip-8.github.io](https://chip-8.github.io/)
- [tobiasvl.github.io](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
- [johnearnest.github.io](http://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html)
- [Timendus/silicon8](https://github.com/Timendus/silicon8/tree/)
- [johnearnest.github.io/chip8Archive](https://johnearnest.github.io/chip8Archive/?sort=platform)

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