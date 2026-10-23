#pragma once
#include <stdint.h>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <future>
#include <memory>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();
    
    template <typename F, typename... Args>
    auto enqueue (F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>;

    private:
    std::vector <std::thread> threads;
    std::condition_variable cv;
    std::mutex mtx;
    std::atomic<bool> stop;
    std::queue <std::function<void()>> tasks;
};

template <typename F, typename... Args>
auto ThreadPool::enqueue (F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> (mtx);

        tasks.emplace(
            [task]() {
                (*task)();
            }
        );
    }
    cv.notify_one();
    return res;
}
