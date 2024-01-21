# Chip8topia

https://en.wikipedia.org/wiki/CHIP-8#Further_reading

## TODO

- [x] Add glad init in Chip8topia.cpp
- [x] Code the Chip8Core
- [x] Code the Chip8topiaUi
- [ ] Code the Chip8topiaDebugger
- [ ] Handle file open dialog for multiple platforms (Windows, Linux, Mac), maybe use a library like GTK
- [ ] Add emulation video
- [ ] Add emulation sound
- [ ] Link emulator inputs and UI inputs
- [ ] Améliorer code imgui des fenêtres avec un système qui contiendrait les enums des fenêtres à afficher, ensuite en
  allant dans un switch case pour chaque enum dans le tableau, on afficherait la fenêtre correspondante

<!--

```mermaid
graph LR

    CD[Chip8topiaDebugger]
    
    Window
    EngineLoop
    
    subgraph Chip8topia
        subgraph Chip8Core
            CPU
            RAM
            PPU
        end
        
        subgraph Chip8topiaUi
            CD
            File
            View
            Design
            Tools
            About
        end
    end
    
    CD --> CPU

    CD --> RAM
    CD --> PPU

```

%%ROM

%% C --> CC
CC --> CPU
CC --> RAM
CC --> ROM
CC --> PPU

C[Chip8topia]

CC[Chip8Core]
CPU[CPU]
RAM[RAM]
ROM[ROM]
PPU[PPU]

-->
