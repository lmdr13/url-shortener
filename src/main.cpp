#include <iostream>
#include "url_repository.hpp"
#include "connection_pool.hpp"
#include "hash.hpp"
#include "base62.hpp"
#include "thread_pool.hpp"
#include "http_server.hpp"
#include <vector>
#include <future>

int main() {
    std::string conn_str = "dbname=db user=myuser password=mypassword host=db port=5432";
    try {
        ConnectionPool cpool(conn_str, 4);
        UrlRepository repo(cpool);
        boost::asio::io_context ioc;
        
        HTTPServer server(ioc, 8080, repo, 4);
        server.start();

        std::cout << "Server started on port 8080 with 4 worker threads...\n";
        
        ioc.run();
    }catch(const std::exception& e) {
        std::cerr << e.what();
    }
}