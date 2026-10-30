#include "connection_pool.hpp"
#include <stdexcept>
ConnectionPool::ConnectionPool (const std::string& conn_str, size_t pool_size) : conn_str(conn_str), pool_size(pool_size) {
    for (size_t i = 0; i < pool_size; i++) {
        auto conn = std::make_shared <pqxx::connection>(conn_str);
        if (!conn->is_open()) {
            throw std::runtime_error("Failed to connect to the database.");
        }
        prepareStatements(*conn);
        pool.emplace(std::move(conn));
    }
}

ConnectionPool::~ConnectionPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;

        while(!pool.empty()) {
            pool.pop();
        }
    }
    cv.notify_all();
}

ConnectionPool::ConnectionProxy ConnectionPool::acquire() {
    std::unique_lock <std::mutex> lock(mtx);

    cv.wait(lock, [this] {
        return !pool.empty() || stop;
    });
    
    if(stop) {
        throw std::runtime_error("Pool is closed.");
    }

    auto conn = pool.front();
    pool.pop();

    return ConnectionProxy(conn, *this);
}

void ConnectionPool::returnConnection(std::shared_ptr<pqxx::connection> conn) {
    std::unique_lock <std::mutex> lock(mtx);

    if(!stop) {
        pool.emplace(std::move(conn));
        lock.unlock();
        cv.notify_one();
    }
}

void ConnectionPool::prepareStatements(pqxx::connection& conn) {
    conn.prepare("insert_url",
        "INSERT INTO urls (original_url, short_key) "
        "VALUES ($1, $2) "
        "ON CONFLICT (short_key) DO NOTHING "
        "RETURNING id;");

    conn.prepare("get_original_url",
        "SELECT original_url FROM urls WHERE short_key = $1;");

    conn.prepare("get_url_details",
        "SELECT id, original_url, short_key"
        "FROM urls WHERE short_key = $1;");
}
