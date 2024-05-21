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
auto getBit(const T& value, const unsigned int& bitIndex) -> bool
{
    return (((value >> bitIndex) & 0x1) == 1);
}

template <typename T>
[[maybe_unused]] void setBit(T& value, const unsigned int& bitIndex)
{
    value |= (1 << bitIndex);
}

template <typename T>
[[maybe_unused]] void resetBit(T& value, const unsigned int& bitIndex)
{
    value &= ~(1 << bitIndex);
}

template <typename T>
[[maybe_unused]] void toggleBit(T& value, const unsigned int& bitIndex)
{
    value ^= (1 << bitIndex);
}

static_assert(sizeof(int8) == 1, "int8 is not 1 byte");
static_assert(sizeof(int16) == 2, "int16 is not 2 bytes");
static_assert(sizeof(int32) == 4, "int32 is not 4 bytes");
static_assert(sizeof(int64) == 8, "int64 is not 8 bytes");

static_assert(sizeof(uint8) == 1, "uint8 is not 1 byte");
static_assert(sizeof(uint16) == 2, "uint16 is not 2 bytes");
static_assert(sizeof(uint32) == 4, "uint32 is not 4 bytes");
static_assert(sizeof(uint64) == 8, "uint64 is not 8 bytes");
