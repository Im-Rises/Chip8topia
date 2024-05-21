#pragma once

#include <binaryLib/binaryLib.h>
#include <string>

namespace schip11_cpu_disassembly
{
    auto disassembleOpcode(const uint16 opcode, bool isModern, bool isLowRes) -> std::string;
}