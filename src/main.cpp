#include <iostream>
#include "url_repository.hpp"
#include "connection_pool.hpp"
#include "hash.hpp"
#include "base62.hpp"
#include "thread_pool.hpp"
#include <vector>
#include <future>

/*
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
*/
/*
int main() {
    std::string conn_str = "dbname=db user=myuser password=mypassword host=127.0.0.1 port=5432";
    std::vector <std::future<std::optional<UrlRecord>>> fres;
    std::vector <std::optional<UrlRecord>> res;
    try {
        ConnectionPool cpool(conn_str, 4);
        UrlRepository repo(cpool);
        ThreadPool pool(4);
        for (int i = 0; i < 26; i++) {
            char a = 'a' + i;
            std::string original_url{a};
            fres.emplace_back(pool.enqueue([original_url, &repo] {
                std::string short_key = encode::to_base62(hash::hash(original_url));
                repo.create(original_url, short_key);
                return repo.getByShortKey(short_key);
            }));
        }

        for (auto& r : fres) {
            res.emplace_back(r.get());
        }

        for (int i = 0; i < 26; i++) {
            char a = 'a' + i;
            std::string original_url{a};
            std::string short_key = encode::to_base62(hash::hash(original_url));
            auto rec = repo.getByShortKey(short_key);
            if (rec && rec->original_url == original_url && rec->short_key == short_key) {
                std::cout << "pass" << std::endl;
            }
            else {
                std::cout << "mistake, original url: " << original_url << std::endl;
            }
        }
    }catch(const std::exception& e) {
        std::cerr << "mistake " << e.what() << std::endl;
    }
}
*/
int main() {
    std::string conn_str = "dbname=db user=myuser password=mypassword host=127.0.0.1 port=5432";
    try {
        ConnectionPool pool (conn_str, 1);
        UrlRepository repo(pool);
        for (int i = 0; i < 26; i++) {
            char a = 'a' + i;
            std::string s{a};
            auto ans = repo.getByShortKey(encode::to_base62(hash::hash(s)));
            if (ans && ans->short_key == encode::to_base62(hash::hash(s))) {
                std::cout << ans->original_url << ' ' << ans->short_key << std::endl;
            }
            else {
                std::cout << "bad";
            }
        }

    }catch (const std::exception& e) {
        std::cerr << "mistake " << e.what() << std::endl;
    }
}