# Chip8topia

<p align="center">
      <img src="README_files/Logos/cmake_logo.png" alt="cmakeLogo" style="height:60px;"/>
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

### Other docs

https://emscripten.org/docs/porting/files/packaging_files.html
https://webgl2fundamentals.org/webgl/lessons/webgl-data-textures.html
https://emscripten.org/docs/porting/files/index.html
https://emscripten.org/docs/porting/files/file_systems_overview.html#file-system-overview
https://emscripten.org/docs/porting/files/Synchronous-Virtual-XHR-Backed-File-System-Usage.html#synchronous-virtual-xhr-backed-file-system-usage
https://github.com/marketplace/actions/upload-release-asset
https://github.com/mymindstorm/setup-emsdk
https://github.com/Armchair-Software/emscripten-browser-file

## Build

### Submodule:

```bash
git clone --recurse-submodules
```

```bash
git submodule update --remote
git submodule update --init --recursive
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
emcmake cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten "-DCMAKE_EXE_LINKER_FLAGS=-s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=[ccall] -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=[_main,_malloc,_free] --preload-file ../../Chip8Games --preload-file ../../shaders" -DCMAKE_BUILD_TYPE=Release
emmake make -C build
```

## TODO

- [x] Add glad init in Chip8topia.cpp
- [x] Add a file explorer to load roms
- [x] Code the Chip8Core
- [x] Code the Chip8topiaUi
- [x] Code the Chip8topiaDebugger
- [x] Handle file open dialog for multiple platforms (Windows, Linux, Mac), maybe use a library like GTK or WxWidgets
- [x] Link emulator inputs and UI inputs
- [x] Finir le code de lecture de rom
- [x] Charger rom
- [x] Linker rom
- [x] Correct window size position for about window
- [x] Add emulation video
- [x] Add emulation speed
- [x] Add fast rom load in debug mode
- [x] Corriger le sp qui pointe à 1 au lieu de 0 quand on push et pas de valeur dans la pile à 0 mais à 1 oui.
- [x] Ajouter la synchronisation entre écran et cpu avec les timers audio et gameplay qui sont synchronisés avec le cpu
- [x] Check all the carry and borrow flags in the opcodes
- [x] Correct collision detection
- [x] Correct the Chip8Ppu issues not rendering correctly (never clear the screen previously, everything was drawn on
  top of
  each other)
- [x] Add a file explorer to load roms
- [x] Add inputs to the emulator
- [x] Throttle the emulator to 60fps
- [x] Add Chip8Ppu clipping
- [x] Add a way to hide all windows
- [x] Prevent hiding the main bar to also hide the windows
- [x] understand and correct the test roms from 5-quirks
- [x] Add inline in the functions declarations of the opcodes (maybe not necessary)
- [x] Add vcpkg support for dependencies
- [x] Correct fps calculation
- [x] Correct the emulation turbo mode (deleted currently)
- [x] Improve the input response by using direct state read rather than callbacks
- [x] Add a way to change the speed of the emulator
- [x] Refactor all imgui windows to look pretty
- [x] Add disassembly
- [x] Change the structure of Chip8topia so it starts the window in a function and not in the constructor
- [x] Add commands and try compilation with emscripten
- [x] Create a macro which will identify release and debug mode and change the window title accordingly _DEBUG and
- [x] Add OpenGL ES support for WebAssembly
- [x] Add .data rom files to the emscripten build
- [x] Integrate data to the assembly like VX and VY becoming V2 and V7 depending on the opcode
- [x] Create actions for github
- [x] Add web rom loading
- [x] Add canvas window size adaptation for the web
- [x] Check if with emscripten it really compile in Release mode
- [x] Replace PlateformIdentifier.h with variable defined in the CMakeLists.txt
- [x] Reformater le code des touches, changer par de vrais noms de touches
- [x] Correct openfiledialog not having good size on the web
- [x] Correct all preprocessing directives for emscripten for key inputs
- [x] Add the rom name on the window title and the Chip8 emulated version
- [x] Add a way to choose the version of the chip8 to emulate
- [x] Correct the minimized window not putting windows at the top when unminimizing
- [x] Correct all the PPU, it should display 8xn sprites, n is a nibble so from 0 to 15 it can be 16 pixels high then
- [x] Correct display in HIRES mode which is doing weird things
- [x] Check the test roms, currently quirks test is perfect for the SCHIPC Modern and not for the SCHIP1.1, need to add
- [x] Copy the core of SCHIP1.1 before the display wait and it will be the perfect SCHIPC Modern (need to change the CPU
- [x] Handle spdlog or iosstream for logging
- [x] Ajouter bouton au desassembleur pour suivre le point de debug. Et surtout en pas à pas faire qu'on le suive tout
  le temps !!!!!!!
- [x] Add break, run, step to the debugger
- [x] Correct break which still break on run
- [x] Finish SCHIPC
- [x] Correct to not link or use spdlog in release mode
- [x] The getPC and getPc are not named the same, correct that
- [x] Dans la section Émulation rajouter une window "émulation info" avec les fps, etc...
- [x] Add a way to change the frequency of the emulator
- [x] Add disassembly for SCHIPC and SCHIP1.1
- [x] Check emscripten canvas resize
- [x] Adapt view to still be in 16:9 ratio
- [x] Correct icon in desktop
- [x] Add modern SCHIP1.1 and test it with the quirks test and scroll test roms
- [x] For the std::vector use ref or move... For the rom loading
- [x] Correct disassembly, we can reach an odd pc value
- [x] Added error system
- [x] Load correct font depending on the version of the chip8
- [x] Correct icon when building in release mode (its all buggy)
- [x] Correct assembly to print hexa values where it should
- [x] Correct disassembly to show the odd or even pc value correctly
- [x] Adding toast system
- [x] Adding icon to the windows

- [ ] Adding toast on rom load etc...
- [ ] Add a way to put front the windows on the main window click

- [ ] Add safe and unsafe code version
- [ ] Add Xo-Chip emulation
- [ ] Utiliser l'event Error pour les fatal errors, utilsier le toast pour les erreurs non fatales et les warnings, et
  utiliser le log pour les logs

- [ ] Replace all try catch with promises from C++23
- [ ] Add different test to not crash even with a bad rom (checks on rom, stack, etc...)
- [ ] Add audio
- [ ] Add save states ? (if yes create a window with an image preview of the save state) ajouter dans la section File de
  main bar
- [ ] Do a general optimization of the code

Why not:

- [-] if none of the roms are loaded, we should display a background with written Chip8 and Im-Rises
- [-] Add a way to change the input keys

OSEF:

- [-] Creer une struct qui définie le nom de la touche shortcut avec le string et le keycode
- [-] Change to init m_chip8topiaUi and m_chip8emulator in the game loop
