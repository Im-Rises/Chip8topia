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
- [-] Creer une struct qui définie le nom de la touche shortcut avec le string et le keycode
- [-] Add imgui.ini to the .data emscripten build
- [x] Add the rom name on the window title and the Chip8 emulated version
- [x] Add a way to choose the version of the chip8 to emulate
- [x] Correct the minimized window not putting windows at the top when unminimizing
- [x] Correct all the PPU, it should display 8xn sprites, n is a nibble so from 0 to 15 it can be 16 pixels high then
- [x] Correct display in HIRES mode which is doing weird things
- [x] Check the test roms, currently quirks test is perfect for the SCHIPC Modern and not for the SCHIP1.1, need to add
- [x] Copy the core of SCHIP1.1 before the display wait and it will be the perfect SCHIPC Modern (need to change the CPU

- [ ] Finish SCHIPC Modern
- [ ] Check emscripten canvas resize
- [ ] Handle spdlog or iosstream for logging

- [ ] Add break, run, step to the debugger
- [ ] Change to init m_chip8topiaUi and m_chip8emulator in the game loop
- [ ] Add emulation sound
- [ ] Add save states ? (if yes create a window with an image preview of the save state)
- [ ] For the std::vector use ref or move... For the rom loading
- [ ] Ajouter bouton au desassembleur pour suivre le point de debug. Et surtout en pas à pas faire qu'on le suive tout
  le temps !!!!!!!
- [ ] Dans la section Émulation rajouter une window "émulation info" avec les fps, etc...
- [ ] Is aucune chip8 n'est chargé alors on draw avec un shader dans Chip8topia. Ce dernier aura l'image en background.
  (Faire un tableau 1d dans le .h avec les valeurs 0 à 1 ou 255.
  Le draw chaque frame]

Check later

- [ ] Add emulation for the SuperChip8 and the XO-Chip ?
- [ ] Add a way to change the used version of the chip8 (ask with a window like the About window, which block user intil
  he decides which version to use) and make it changeable before running a game and when the game is running (restart
  the game when the version is changed while the game is running)
- [ ] Add a way to change the frequency of the emulator
- [ ] Faire une fenêtre imgui quand on démarre pour sélectionner la version de chip8
- [ ] Faire héritage cpp pour cpu et pour selection et peut être Chip8Core
- [ ] Faire un switch case qui démarre la version de Core sélectionné avec la création de Chip8Core m_core; en
  tant que unique_ptr
- [ ] Pour les load rom être sûr de bien utiliser std::move pour les vecteurs et des && pour les paramètres de
  fonctions
- [ ] Pour la partie emulation faire des surcharges de fonctions avec comme paramètre const SuperChip8Core& core,
  const Chip8Core& core, const XOChip8Core& core, de cette façon on pourra définir trois fonctions différentes pour
  chaque version de Core et le prendra autmaotiquement lors de l'appel ?
- [ ] Faire des fonctions virtuel et les override dans chaque cpu ! Pas pure virtual, pour que si c'est pas override ça
  appelle "assert" !

## Dependencies

- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [stb](https://github.com/nothings/stb)
- [ImGui](https://github.com/ocornut/imgui)
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)
- [ImGui Memory Editor](https://github.com/ocornut/imgui_club/tree/main/imgui_memory_editor)
- [Vcpkg](https://vcpkg.io/en)

### Other docs

https://emscripten.org/docs/porting/files/packaging_files.html
https://webgl2fundamentals.org/webgl/lessons/webgl-data-textures.html
https://emscripten.org/docs/porting/files/index.html
https://emscripten.org/docs/porting/files/file_systems_overview.html#file-system-overview
https://emscripten.org/docs/porting/files/Synchronous-Virtual-XHR-Backed-File-System-Usage.html#synchronous-virtual-xhr-backed-file-system-usage
https://github.com/marketplace/actions/upload-release-asset
https://github.com/mymindstorm/setup-emsdk
https://github.com/Armchair-Software/emscripten-browser-file

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
emcmake cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DVCPKG_TARGET_TRIPLET=wasm32-emscripten "-DCMAKE_EXE_LINKER_FLAGS=-s USE_GLFW=3 -s FULL_ES3=1 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=[ccall] -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=[_main,_malloc,_free] --preload-file ../../Chip8Games --preload-file ../../shaders" -DCMAKE_BUILD_TYPE=Release
emmake make -C build
```
