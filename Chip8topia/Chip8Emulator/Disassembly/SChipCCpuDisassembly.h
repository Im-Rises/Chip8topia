#pragma once

#include <binaryLib/binaryLib.h>
#include <string>

namespace SChipCCpuDisassembly
{
    auto disassembleOpcode(const uint16 opcode, bool isLowRes) -> std::string;
}