cmake_minimum_required(VERSION 3.15)

project(url-shortener)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(boost_asio CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(xxHash 0.8 CONFIG REQUIRED)

add_executable(main src/main.cpp)

add_library(hash STATIC src/hash.cpp)
target_include_directories(hash PUBLIC include)
target_link_libraries(hash PUBLIC xxHash::xxhash)

add_library(base62 STATIC src/base62.cpp)
target_include_directories(base62 PUBLIC include)

add_library(ThreadPool STATIC src/thread_pool.cpp)
target_include_directories(ThreadPool PUBLIC include)

add_library(ConnectionPool STATIC db/connection_pool.cpp)
target_include_directories(ConnectionPool PUBLIC db)
target_link_libraries(ConnectionPool PUBLIC libpqxx::pqxx)

add_library(UrlRepository STATIC db/url_repository.cpp)
target_include_directories(UrlRepository PUBLIC db)
target_link_libraries(UrlRepository PUBLIC ConnectionPool libpqxx::pqxx)

target_link_libraries(main 
    PRIVATE 
        hash 
        base62 
        ThreadPool
        ConnectionPool
        UrlRepository
)