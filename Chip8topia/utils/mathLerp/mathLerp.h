#pragma once

// Lerp between two values
template <typename T>
[[nodiscard]] auto mathLerp(const T& a, const T& b, const float t) -> T
{
    return a + t * (b - a);
}
