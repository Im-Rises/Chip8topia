#pragma once

#include <binaryLib/binaryLib.h>
#include <string>

namespace Chip8CpuDisassembly {
    auto disassembleOpcode(const uint16 opcode) -> std::string;
}