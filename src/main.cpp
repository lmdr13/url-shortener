#include <hash.hpp>
#include <base62.hpp>
#include <thread_pool.hpp>
#include <string>
#include <iostream>
#include <chrono>

int main() {
    ThreadPool pool(4);
    std::vector <std::string> vec = {"qwe", "bgoeh", "jkqn", "goadffv", "3453", "fddlk", "dsghtiubg"};
    std::vector <std::future<std::string>> fres;
    std::vector <std::string> res;
    for (auto& s : vec) {
        fres.emplace_back(
            pool.enqueue([s] {
                return encode::to_base62(hash::hash(s));
            })
        );
    }

    for (auto&& f : fres) {
        res.push_back(f.get());
    }
    for (std::string s : res) {
        std::cout << s << '\n';
    }
}