#include <iostream>
#include "url_repository.hpp"
#include "connection_pool.hpp"
#include "hash.hpp"
#include "base62.hpp"
#include "thread_pool.hpp"
#include <vector>
#include <future>

int main() {
    std::string conn_str = "dbname=db user=myuser password=mypassword host=127.0.0.1 port=5432";
    std::vector <std::string> vec = {"1", "fdgfdsfd", "343498gr", "bgiwoesrbg", "srfdrgs", "98hw948hto", "sgbljsd", "94830hq9", "d "};
    std::vector <std::future<std::optional<UrlRecord>>>fres;
    try {
        ConnectionPool cpool(conn_str, 4);
        UrlRepository repo(cpool);
        ThreadPool thpool(4);
        for (auto& s : vec) {
            fres.emplace_back(thpool.enqueue([s, &repo] {
                std::string short_key = encode::to_base62(hash::hash(s));
                repo.create(s, short_key);
                return repo.getByShortKey(short_key);
            }));
        }
        for (auto& r : fres) {
            auto rep = r.get();
            if (rep) {
                std::cout << "original_url: " << rep->original_url << " short_key: " << rep->short_key << std::endl;
            }
            else {
                std::cerr << "mistake" << std::endl;
            }
        }
    }catch(const std::exception& e) {
        std::cerr << e.what();
    }
}