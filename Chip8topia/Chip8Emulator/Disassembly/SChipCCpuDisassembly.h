#pragma once

#include <binaryLib/binaryLib.h>
#include <string>

namespace schipc_cpu_disassembly
{
    auto disassembleOpcode(const uint16 opcode, bool isLowRes) -> std::string;
}