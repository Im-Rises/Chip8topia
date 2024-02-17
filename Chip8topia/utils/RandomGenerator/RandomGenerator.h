#pragma once

#include <random>

class RandomGenerator {
public:
    RandomGenerator(int min, int max) : m_randomDistribution(min, max),
                                        m_randomEngine(std::random_device{}()) {
    }
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator(RandomGenerator&&) = delete;
    auto operator=(const RandomGenerator&) -> RandomGenerator& = delete;
    auto operator=(RandomGenerator&&) -> RandomGenerator& = delete;
    ~RandomGenerator() = default;

public:
    [[nodiscard]] auto generateRandomNumber() -> int {
        return m_randomDistribution(m_randomEngine);
    }

private:
    std::mt19937 m_randomEngine;
    std::uniform_int_distribution<int> m_randomDistribution;
};
