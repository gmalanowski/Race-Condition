#include "ThreadPool.h"
#include <utility>      // std::move
#include <exception>    // std::exception

// Start worker threads
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    workers.reserve(numThreads);
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] { worker_loop(); });
    }
}

// Worker main loop: wait for work; exit when stop && tasks empty
void ThreadPool::worker_loop() {
    for (;;) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] {
                return stop || !tasks.empty();
            });

            if (stop && tasks.empty())
                return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        // Never let an exception kill the worker thread
        try {
            task();
        } catch (...) {
            continue;
        }
    }
}

// Stop pool and join all workers
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();

    for (std::thread &worker : workers) {
        if (worker.joinable())
            worker.join();
    }
}
