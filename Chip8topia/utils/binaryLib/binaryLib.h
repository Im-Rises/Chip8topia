#pragma once

using int8 = signed char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;


// constexpr int8 INT8_MAX = std::numeric_limits<int8>::max();
// constexpr int16 INT16_MAX = std::numeric_limits<int16>::max();
// constexpr int32 INT32_MAX = std::numeric_limits<int32>::max();
// constexpr int64 INT64_MAX = std::numeric_limits<int64>::max();
// constexpr uint8 UINT8_MAX = std::numeric_limits<uint8>::max();
// constexpr uint16 UINT16_MAX = std::numeric_limits<uint16>::max();
// constexpr uint32 UINT32_MAX = std::numeric_limits<uint32>::max();
// constexpr uint64 UINT64_MAX = std::numeric_limits<uint64>::max();
//
// constexpr int8 INT8_MIN = std::numeric_limits<int8>::min();
// constexpr int16 INT16_MIN = std::numeric_limits<int16>::min();
// constexpr int32 INT32_MIN = std::numeric_limits<int32>::min();
// constexpr int64 INT64_MIN = std::numeric_limits<int64>::min();
// constexpr uint8 UINT8_MIN = std::numeric_limits<uint8>::min();
// constexpr uint16 UINT16_MIN = std::numeric_limits<uint16>::min();
// constexpr uint32 UINT32_MIN = std::numeric_limits<uint32>::min();
// constexpr uint64 UINT64_MIN = std::numeric_limits<uint64>::min();


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

// template <typename T>
// auto toBitString(const T& value) -> std::string {
//     return std::bitset<sizeof(T) * 8>(value).to_string();
// }
