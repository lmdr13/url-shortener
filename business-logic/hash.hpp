#pragma once
#include <xxhash.h>
#include <stdint.h>
#include <string>

namespace hash {
    uint64_t hash(std::string str);
}