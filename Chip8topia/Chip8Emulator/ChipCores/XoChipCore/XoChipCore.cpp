#include "XoChipCore.h"

#include "Core/XoChipCpu.h"
#include "Core/XoChipPpu.h"

XoChipCore::XoChipCore(Chip8Frequency cpuClockFrequency) : Chip8CoreBase(cpuClockFrequency, std::make_unique<XoChipCpu>(), std::make_shared<XoChipPpu>())
{
}
