#include <hash.hpp>
#include <base62.hpp>
#include <string>
#include <iostream>

int main() {
    std::string s = "ya.ru";
    std::cout << encode::to_base62(hash::hash(s));
}