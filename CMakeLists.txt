cmake_minimum_required(VERSION 3.15)

project(url-shortener)

find_package(boost_asio CONFIG REQUIRED)

find_package(libpqxx CONFIG REQUIRED)

find_package(xxHash 0.8 CONFIG REQUIRED)

add_executable(main src/main.cpp)

add_library(hash STATIC src/hash.cpp)
add_library(base62 STATIC src/base62.cpp)
add_library(ThreadPool STATIC src/thread_pool.cpp)

target_include_directories(hash PUBLIC include)
target_include_directories(base62 PUBLIC include)
target_include_directories(ThreadPool PUBLIC include)

target_link_libraries(main 
    PRIVATE 
        hash 
        base62 
        ThreadPool 
        Boost::asio
        libpqxx::pqxx
        xxHash::xxhash
)