#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; i++) {
        threads.emplace_back(
            [this] {
                while(true) {
                    std::function <void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx);

                        cv.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            }
        );
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    cv.notify_all();
    for (auto& th : threads) {
        th.join();
    }
}