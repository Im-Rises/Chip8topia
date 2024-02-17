# Chip8topia

https://en.wikipedia.org/wiki/CHIP-8#Further_reading

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

http://tobiasvl.github.io/blog/write-a-chip-8-emulator/

https://www.laurencescotford.net/2020/07/19/chip-8-on-the-cosmac-vip-arithmetic-and-logic-instructions/

https://github.com/Timendus/chip8-test-suite

https://www.laurencescotford.net/2020/07/19/chip-8-on-the-cosmac-vip-drawing-sprites/

https://chip8.gulrak.net/

https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy1-binary-or

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
- [x] Correct the Ppu issues not rendering correctly (never clear the screen previously, everything was drawn on top of
  each other)
- [x] Add a file explorer to load roms
- [x] Add inputs to the emulator
- [x] Throttle the emulator to 60fps
- [x] Add Ppu clipping
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
- [ ] Add OpenGL ES support for WebAssembly
- [ ] Create a macro which will identify release and debug mode and change the window title accordingly _DEBUG and
  NDEBUG
- [ ] Faire des fonctions virtuel et les override dans chaque cpu ! Pas pure virtual, pour que si c'est pas override ça
  appelle "assert" !
- [ ] Replace PlateformIdentifier.h with variable defined in the CMakeLists.txt
- [ ] Add emulation sound
- [ ] Create a class Memory with the memory and the value of 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
- [ ] Add emulation for the SuperChip8 and the XO-Chip ?
- [ ] Add a way to change the used version of the chip8 (ask with a window like the About window, which block user intil
  he decides which version to use) and make it changeable before running a game and when the game is running (restart
  the game when the version is changed while the game is running)
- [ ] Add a way to change the frequency of the emulator
- [ ] Faire une fenêtre imgui quand on démarre pour sélectionner la version de chip8
- [ ] Faire héritage cpp pour cpu et pour selection et peut être Chip8Core
- [ ] Add save states ?
- [ ] Créer une classe ImGuiMenuItemWindowList. Améliorer code imgui des fenêtres avec un système qui contiendrait les
  enums des fenêtres à afficher, ensuite en
  allant dans un switch case pour chaque enum dans le tableau, on afficherait la fenêtre correspondante. Rajouter class
  ImGui (Créer une classe ImGuiMenuItemWindowList). Faire une structure qui prend en paramètre un array seulement
  movable

## Dependencies

- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [stb](https://github.com/nothings/stb)
- [ImGui](https://github.com/ocornut/imgui)
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)
- [ImGui Memory Editor](https://github.com/ocornut/imgui_club/tree/main/imgui_memory_editor)
- [Vcpkg](https://vcpkg.io/en)

## Commands

### Submodule:

```bash
git submodule update --remote
git submodule update --init --recursive
```

### Vcpkg:

In order to use vcpkg with CMake outside of an IDE, you can use the toolchain file:

```bash
cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

Then build with:

```bash
cmake --build [build directory]
```

### Emscripten

```bash
emcmake cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten "-DCMAKE_EXE_LINKER_FLAGS=-s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1" -DCMAKE_BUILD_TYPE=Release  
emmake make -C build // or cd build && make
```
