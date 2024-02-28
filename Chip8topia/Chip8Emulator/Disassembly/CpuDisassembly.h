#pragma once

#include <binaryLib/binaryLib.h>
#include <string>

namespace CpuDisassembly {
    auto disassembleOpcode(const uint16 opcode) -> std::string;
}