#pragma once
#include <pqxx/pqxx>
#include <string>
#include <queue>
#include <condition_variable>
#include <memory>
#include <mutex>

class ConnectionPool {
public:
    explicit ConnectionPool(const std::string& conn_str, size_t pool_size);
    ~ConnectionPool();
    
    class ConnectionProxy {
        public:
        ConnectionProxy(std::shared_ptr<pqxx::connection> conn, ConnectionPool& pool)
        : conn(std::move(conn)), pool(pool) {}

        ~ConnectionProxy() {
            if (conn) {
                pool.returnConnection(std::move(conn));
            }
        }
        
        ConnectionProxy(const ConnectionProxy&) = delete;
        ConnectionProxy& operator=(const ConnectionProxy&) = delete;
        
        ConnectionProxy(ConnectionProxy&&) noexcept = default;
        ConnectionProxy& operator=(ConnectionProxy&&) noexcept = default;

        pqxx::connection& operator*() { return *conn; }
        pqxx::connection* operator->() { return conn.get(); }

        private:
        std::shared_ptr <pqxx::connection> conn;
        ConnectionPool& pool;
    };
    
    ConnectionProxy acquire();
    
private:

    friend class ConnectionProxy;
    void returnConnection(std::shared_ptr<pqxx::connection> conn);

    static void prepareStatements(pqxx::connection& conn);
    std::string conn_str;
    size_t pool_size;
    std::queue <std::shared_ptr<pqxx::connection>> pool;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
};