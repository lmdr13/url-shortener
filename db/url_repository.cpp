#include "url_repository.hpp"
#include <iostream>

UrlRepository::UrlRepository(ConnectionPool& pool) : pool(pool) {}

std::optional <UrlRecord> UrlRepository::create(const std::string& original_url, const std::string& short_key) {
    try {
        auto conn_proxy = pool.acquire();

        pqxx::work txn(*conn_proxy);
        pqxx::result res = txn.exec_prepared("insert url", original_url, short_key);

        if (res.empty()) {
            txn.abort(); 
            return std::nullopt; 
        }

        txn.commit();

        UrlRecord record;
        record.id = res[0]["id"].as<int64_t>();
        record.original_url = original_url;
        record.short_key = short_key;

        return record; 


    } catch (const std::exception& e) {
        std::cerr << "Database Exception: " << e.what() << std::endl;
        return std::nullopt;
    };
}

std::optional<std::string> UrlRepository::getOriginalUrl(const std::string& short_key) {
    try {
        auto conn_proxy = pool.acquire();

        pqxx::read_transaction txn(*conn_proxy);
        pqxx::result res = txn.exec_prepared("get_original_url", short_key);
        if (res.empty()) {
            return std::nullopt;
        }

        return res[0]["original_url"].as<std::string>();

    } catch(const std::exception& e) {
        std::cerr << "Database Exception: " << e.what() << std::endl;
        return std::nullopt;
    }
}