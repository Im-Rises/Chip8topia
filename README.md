# Chip8topia

<p align="center">
      <img src="https://user-images.githubusercontent.com/59691442/183268126-b3d19e66-8f2d-463a-805e-ae6ef7cc6c01.png" alt="cmakeLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cppLogo" style="height:60px;"/>
      <img src="https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl" alt="openglLogo" style="height:60px;"/>
</p>

## Description

Chip8topia is a Chip8 emulator written in C++ for Windows, Linux, macOS and WebAssembly. It uses OpenGL/WebGL for the
rendering
and ImGui for the UI.
The application is compiled with CMake and uses Vcpkg for most dependencies.
For the WebAssembly version, it uses Emscripten to compile with vcpkg for most dependencies as well.

It can emulate the original Chip8, the SCHIP1.1, SCHIPC and Xo-Chip.

Platforms:

- [x] Windows
- [x] Linux
- [x] macOS
- [x] WebAssembly

Emulated consoles:

- [x] Chip8
- [x] SCHIP1.1
- [x] SCHIPC
- [x] Xo-Chip

## Screenshots

| Chip8topia Tools                                                                                                      | Chip8topia Debug Tools                                                                                                 |
|-----------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------|
| ![chip8topia-main-tools](https://github.com/Im-Rises/Chip8topia/assets/59691442/0024eddd-8af2-49fa-89e4-6b4e11660712) | ![chip8topia-debug-tools](https://github.com/Im-Rises/Chip8topia/assets/59691442/44ed037c-4921-4c83-9411-54c9a0b51f53) |

| Invaders (Chip8)                                                                                                         | Car (SCHIP)                                                                                                         | T8NKS (Xo-Chip)                                                                                                       |                                                                                                                   
|--------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| ![invaders-game-screenshot](https://github.com/Im-Rises/Chip8topia/assets/59691442/3e5a2078-2cf7-4929-94c2-ea42cea24efd) | ![car-game-screenshot](https://github.com/Im-Rises/Chip8topia/assets/59691442/13774fed-1ce4-4ab6-b73d-40aa374661a0) | ![t8nks-game-screenshot](https://github.com/Im-Rises/Chip8topia/assets/59691442/e4ab6b1f-e5a4-485f-a26b-dab63e20aff1) |

| Alien Inv8sion Title Screen (Xo-Chip Octo)                                                                                 | Alien Inv8sion Gameplay (Xo-Chip Octo)                                                                                  |
|----------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------|
| ![alien-inv8sion-titlescreen](https://github.com/Im-Rises/Chip8topia/assets/59691442/71eb63c2-4716-49ba-868b-1525f593529e) | ![alien-inv8sion-gameplay](https://github.com/Im-Rises/Chip8topia/assets/59691442/b1c7441a-6045-4cf3-bb98-7fd6d03835d3) |

## Features

### Main

- [x] Desktop and Web version
- [x] Chip8, SCHIP1.1, SCHIPC and Xo-Chip support
- [x] Pause/Resume/Restart the emulation
- [x] Change the emulation speed
- [x] Change the game colors
- [x] Fullscreen mode
- [x] Rom loader
- [x] Sound

### Debug Tools

- [x] Registers viewer
- [x] Stack viewer
- [x] Memory Editor
- [x] Virtual Keypad
- [x] Disassembly
- [x] Disassembly controls (break, step, run, reset)
- [x] Breakpoints
- [x] Planes viewer

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

## TODO

Major:

- [ ] Add a try catch (check Chip8CoreBase todo)
- [ ] Correct Chip8 sound emulation issue in desktop and crash in web version
- [ ] Cleanup the full handling of the sound emulation
- [ ] Add Xo-Chip audio
- [ ] Replace the input handler singleton with another pattern...

Minor:

- [ ] Depending on the rom extension, load the correct console (Chip8, SCHIP, SCHIPC, Xo-Chip)
- [ ] Restarting a game when playing big xo-chip games will not reload the game from the start (we should reset memory
  and reload the game rom)
- [ ] Improve disassembly (check TODO in the Chip8topiaDisassembly.cpp and Chip8topiaDisassembler)

<!-- Other
Other tasks:
- [ ] Use cpack to create a release (also update the GitHub Actions) https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html
- [ ] Add a way to change the input keys
- [ ] Add a background when no rom is loaded

Profiling logs:
- [x] Issue with imgui viewport when vsync enabled and window out of the sdl2/glfw main window it doubles the time of the swap buffers, like its doing it two times (disabled completely vsync and use of std::this_thread::sleep_for(std::chrono::milliseconds(16)) in the main loop)
- [x] LD_Vx_K is somehow very very slow ??? (corrected by not iterating over all the keys but only checking if the previous bitset of the keys is different from the current one)
- [x] Breakpoints in Chip8Emulator is slow (stop using a std::set when checking if we should break on the current PC and use of an array)
-->

## Documentation

- [chip8.gulrak.net](https://chip8.gulrak.net/)
- [chip-8.github.io](https://chip-8.github.io/)
- [tobiasvl.github.io](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
- [Timendus/chip8-test-suite](https://github.com/Timendus/chip8-test-suite)
- [Timendus/silicon8](https://github.com/Timendus/silicon8/tree/)
- [Timendus/alien-inv8sion](https://github.com/Timendus/alien-inv8sion)
- [johnearnest.github.io](http://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html)
- [johnearnest.github.io/chip8Archive](https://johnearnest.github.io/chip8Archive/?sort=platform)
- [johnearnest.github.io/Octo](https://johnearnest.github.io/Octo/)
- [jacobsebek SDL2 audio callack demo](https://gist.github.com/jacobsebek/10867cb10cdfccf1d6cfdd24fa23ee96)

[//]: # (- [devernay.free.fr]&#40;http://devernay.free.fr/hacks/chip8/C8TECH10.HTM&#41;)

## Rom links

- [https://github.com/JohnEarnest/chip8Archive](JohnEarnest/chip8Archive)
- [https://archive.org/details/Chip-8RomsThatAreInThePublicDomain](Chip-8RomsThatAreInThePublicDomain)

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
- [SDL2](https://www.libsdl.org/)
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
emcmake cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten "-DCMAKE_EXE_LINKER_FLAGS=-s USE_SDL=2 -s FULL_ES3=1 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=[ccall] -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=[_main,_malloc,_free] --preload-file ../../Chip8Games --preload-file ../../shaders --preload-file ../../fonts" -DCMAKE_BUILD_TYPE=Release
```

then build with:

```bash
emmake make -C [build directory]
```

## GitHub Actions

[![CMake Vcpkg Publish Binaries](https://github.com/Im-Rises/Chip8topia/actions/workflows/cmake-vcpkg-publish-binaries.yml/badge.svg)](https://github.com/Im-Rises/Chip8topia/actions/workflows/cmake-vcpkg-publish-binaries.yml)
[![CMake Vcpkg Emscripten Publish](https://github.com/Im-Rises/Chip8topia/actions/workflows/cmake-vcpkg-emscripten-publish.yml/badge.svg)](https://github.com/Im-Rises/Chip8topia/actions/workflows/cmake-vcpkg-emscripten-publish.yml)

The project uses GitHub Actions to build and publish the desktop builds to the GitHub release page and publish the
WebAssembly build to GitHub Pages.

## Contributors

Quentin MOREL :

- @Im-Rises
- <https://github.com/Im-Rises>

[![GitHub contributors](https://contrib.rocks/image?repo=Im-Rises/Chip8topia)](https://github.com/Im-Rises/Chip8topia/graphs/contributors)
