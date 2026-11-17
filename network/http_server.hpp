#pragma once

#include <boost/asio.hpp>
#include "thread_pool.hpp"
#include "url_repository.hpp"



using boost::asio::ip::tcp;

class HTTPServer {
public:
    HTTPServer(boost::asio::io_context& ioc, short port, UrlRepository& repo, size_t num_threads);
    void start();

private:
    void do_accept();
    static void handle_request(tcp::socket socket, UrlRepository& db);

    tcp::acceptor acceptor;
    UrlRepository& repo;
    ThreadPool thread_pool;
};
