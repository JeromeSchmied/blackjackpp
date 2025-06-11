#pragma once

#include <random>

namespace rng {
    inline std::random_device rd;
    inline std::mt19937 custom_random(rd());
}  // namespace rng
