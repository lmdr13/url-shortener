cmake_minimum_required(VERSION 3.15)

project(url-shortener)

add_executable(main src/main.cpp)


add_library(hash STATIC src/hash.cpp include/hash.hpp)
add_library(base62 STATIC src/base62.cpp include/base62.hpp)
add_library(ThreadPool STATIC src/thread_pool.cpp include/thread_pool.hpp)

target_include_directories(base62 PUBLIC include)
target_include_directories(hash PUBLIC include)
target_include_directories(ThreadPool PUBLIC include)

target_link_libraries(main PRIVATE hash)
target_link_libraries(main PRIVATE base62)
target_link_libraries(main PRIVATE ThreadPool)

find_package(xxHash CONFIG REQUIRED)
message(STATUS "==== ГДЕ НАШЕЛСЯ xxHash: ${xxHash_INCLUDE_DIRS} ====")
target_link_libraries(main PRIVATE xxHash::xxhash)

#find_package(xxHash 0.8 CONFIG REQUIRED)
#target_link_libraries(main PRIVATE xxHash::xxhash)