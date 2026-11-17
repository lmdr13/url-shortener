#pragma once
#include <string>
#include <algorithm>
#include <stdint.h>

namespace encode {
    std::string to_base62(uint64_t value);
}