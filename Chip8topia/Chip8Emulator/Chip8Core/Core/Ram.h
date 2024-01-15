#pragma once

// Get the screen data from here and copy it to the screen buffer in Ppu.h

#include "binaryLib/binaryLib.h"

class Ram {
private:
    uint8 m_memory[0x1000];
};
