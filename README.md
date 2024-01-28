# Chip8topia

https://en.wikipedia.org/wiki/CHIP-8#Further_reading

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

## TODO

- [x] Add glad init in Chip8topia.cpp
- [x] Add a file explorer to load roms
- [x] Code the Chip8Core
- [x] Code the Chip8topiaUi
- [ ] Code the Chip8topiaDebugger
- [x] Handle file open dialog for multiple platforms (Windows, Linux, Mac), maybe use a library like GTK or WxWidgets
- [ ] Add emulation video
- [ ] Add emulation sound
- [x] Link emulator inputs and UI inputs
- [ ] Améliorer code imgui des fenêtres avec un système qui contiendrait les enums des fenêtres à afficher, ensuite en
  allant dans un switch case pour chaque enum dans le tableau, on afficherait la fenêtre correspondante
- [ ] Rajouter class ImGui (Créer une classe ImGuiMenuItemWindowList)
- [?] Faire une structure qui prend en paramètre un array seulement movable
- [ ] Créer une classe ImGuiMenuItemWindowList
- [ ] Finir le code de lecture de rom
- [ ] Charger rom
- [ ] Linker rom
- [ ] Correct window size position for about window

## Dependencies

- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [ImGui](https://github.com/ocornut/imgui)
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)
- [ImGui Memory Editor](https://github.com/ocornut/imgui_club/tree/main/imgui_memory_editor)
- [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/)
