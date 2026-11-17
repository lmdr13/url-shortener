#pragma once

#include "models.hpp"
#include "connection_pool.hpp"
#include <optional>
#include <string>

class UrlRepository {
public:
    explicit UrlRepository(ConnectionPool& pool);
    std::optional<UrlRecord> create(const std::string& original_url, const std::string& short_key);

    std::optional<std::string> getOriginalUrl(const std::string& short_key);

    std::optional<UrlRecord> getByShortKey(const std::string& short_key);

private:
    ConnectionPool& pool;
};
