#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <stdexcept>
#include <atomic>

class ThreadPool {
public:
    // Constructor
    ThreadPool(size_t numThreads);
    // Destructor
    ~ThreadPool();

    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        // Get the return type of the function
        using return_type = decltype(f(args...));

        // Create a packaged_task to wrap the function and its future result.
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        // Get the future from the packaged_task
        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // Don't allow enqueueing after stopping
            if(stop) {
                throw std::runtime_error("submit on stopped ThreadPool");
            }

            // Push a lambda that executes the packaged_task into the queue
            tasks.emplace([task](){ (*task)(); });
        }

        // Notify one waiting worker thread that there's a new task
        condition.notify_one();
        return res;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    void worker_loop();
};

#endif //THREADPOOL_H
