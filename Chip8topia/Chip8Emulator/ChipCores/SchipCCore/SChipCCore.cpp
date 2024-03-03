#include "SChipCCore.h"

#include "Core/SChipCCpu.h"
#include "Core/SChipCPpu.h"

SChipCCore::SChipCCore(unsigned int cpuClockFrequency) : Chip8CoreBase(cpuClockFrequency, std::make_unique<SChipCCpu>(), std::make_shared<SChipCPpu>()) {
}
