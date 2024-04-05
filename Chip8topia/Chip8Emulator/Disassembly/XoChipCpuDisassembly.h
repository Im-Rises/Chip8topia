#pragma once

#include <binaryLib/binaryLib.h>
#include <string>

namespace XoChipCpuDisassembly
{
    auto disassembleOpcode(const uint16 opcode) -> std::string;
}