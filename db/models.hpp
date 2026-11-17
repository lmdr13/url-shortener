#pragma once
#include <string>
#include <stdint.h>

struct UrlRecord {
    uint64_t id = 0;
    std::string original_url;
    std::string short_key;
};