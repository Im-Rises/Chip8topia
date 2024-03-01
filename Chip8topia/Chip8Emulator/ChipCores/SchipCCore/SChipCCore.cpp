#include "SChipCCore.h"

#include "Core/SChipCCpu.h"
#include "Core/SChipCPpu.h"

SChipCCore::SChipCCore() : Chip8CoreBase(std::make_unique<SChipCCpu>(), std::make_shared<SChipCPpu>()) {
}
