#include "ThreadPool.h"

// Constructor implementation
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            this->worker_loop();
        });
    }
}

// The worker thread's main loop
void ThreadPool::worker_loop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(lock, [this] {
                return this->stop || !this->tasks.empty();
            });

            if (this->stop && this->tasks.empty()) {
                return;
            }

            task = std::move(this->tasks.front());
            this->tasks.pop();
        }
        task();
    }
}

// Destructor implementation
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}
