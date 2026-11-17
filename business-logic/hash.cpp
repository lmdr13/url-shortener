#include "hash.hpp"

namespace hash {
    uint64_t hash(std::string str) {
        return XXH3_64bits(str.data(), str.size());
    }
}