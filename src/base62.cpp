#include "base62.hpp"

namespace encode {
    const std::string CODES = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string to_base62(uint64_t value) {
        std::string res;
        do {
            res += CODES[value%62];
            value/=62;
        } while (value > 0);
        
        reverse(res.begin(), res.end());
        return res;
    }
}