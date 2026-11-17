#include "http_server.hpp"
#include <memory>
#include <boost/beast.hpp>
#include <iostream>
#include "base62.hpp"
#include "hash.hpp"

namespace beast = boost::beast;
namespace http = beast::http;

HTTPServer::HTTPServer(boost::asio::io_context& ioc, short port, UrlRepository& repo, size_t num_threads)
    : acceptor(ioc, tcp::endpoint(tcp::v4(), port)), repo(repo), thread_pool(num_threads) {}

void HTTPServer::start() {
    do_accept();
}

void HTTPServer::do_accept() {
    acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                thread_pool.enqueue([sock = std::move(socket), &repo = this->repo]() mutable {
                    handle_request(std::move(sock), repo);
                });
            }
            do_accept(); 
        });
}

void HTTPServer::handle_request(tcp::socket socket, UrlRepository& repo) {
    try {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        http::response<http::string_body> res;
        res.version(req.version());
        res.keep_alive(false);

        if (req.method() == http::verb::get) {
            std::string target(req.target());
            std::string key = target.substr(1); 

            auto original_url = repo.getOriginalUrl(key);
            if (original_url) {
                res.result(http::status::found); 
                res.set(http::field::location, *original_url);
            } else {
                res.result(http::status::not_found);
                res.body() = "URL not found";
            }
        }
        else if (req.method() == http::verb::post) {
            std::string original_url = req.body();
            std::string key = encode::to_base62(hash::hash(original_url));
            
            if (repo.create(original_url, key)) {
                res.result(http::status::ok);
                res.body() = "http://localhost:8080/" + key;
            } else {
                res.result(http::status::internal_server_error);
                res.body() = "Failed to save to repo";
            }
        } else {
            res.result(http::status::bad_request);
            res.body() = "Unsupported HTTP-method";
        }

        res.prepare_payload();
        http::write(socket, res);
        
        boost::system::error_code ec;
        socket.shutdown(tcp::socket::shutdown_send, ec);
    } catch (const std::exception& e) {
        std::cerr << "Request handling error: " << e.what() << std::endl;
    }
}