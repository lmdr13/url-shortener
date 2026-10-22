cmake_minimum_required(VERSION 3.15)

project(url-shortener)

add_executable(main src/main.cpp)


add_library(hash STATIC src/hash.cpp include/hash.hpp)
add_library(base62 STATIC src/base62.cpp include/base62.hpp)
target_include_directories(base62 PUBLIC include)
target_include_directories(hash PUBLIC include)

target_link_libraries(main PRIVATE hash)
target_link_libraries(main PRIVATE base62)

find_package(xxHash 0.8 CONFIG REQUIRED)
target_link_libraries(main PRIVATE xxHash::xxhash)