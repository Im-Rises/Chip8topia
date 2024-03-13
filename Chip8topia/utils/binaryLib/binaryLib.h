#pragma once

using int8 = signed char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

template <typename T>
auto getBit(const T& value, const int& bitNumber) -> bool {
    return (((value >> bitNumber) & 0x1) == 1);
}

template <typename T>
void setBit(T& value, const int& bitNumber) {
    value |= (1 << bitNumber);
}

template <typename T>
void resetBit(T& value, const int& bitNumber) {
    value &= ~(1 << bitNumber);
}

template <typename T>
void toggleBit(T& value, const int& bitNumber) {
    value ^= (1 << bitNumber);
}
