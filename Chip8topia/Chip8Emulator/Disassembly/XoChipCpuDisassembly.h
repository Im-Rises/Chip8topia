#pragma once

#include <binaryLib/binaryLib.h>
#include <string>

namespace xochip_cpu_disassembly
{
    auto disassembleOpcode(const uint16 opcode) -> std::string;
}