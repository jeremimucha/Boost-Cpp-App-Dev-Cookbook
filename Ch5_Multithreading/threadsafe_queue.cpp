#include <iostream>
#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cassert>

class work_queue
{
public:
    using task_type = std::function<void()>;

    // add a task to the queue
    void push_task(const task_type& task)
    {
        // std::unique_lock<std::mutex> lock{tasks_mutex_};
        std::lock_guard<std::mutex> lock{tasks_mutex_};
        tasks_.push_back(task);
        // lock.unlock();
        cond_.notify_one();
    }

    // Non-blocking function for getting a, possibly empty, task.
    task_type try_pop_task()
    {
        task_type ret;
        std::lock_guard<std::mutex> lock{tasks_mutex_};
        if(!tasks_.empty()){
            ret = tasks_.front();
            tasks_.pop_front();
        }
        return ret;
    }

    // blocking variant of getting a task from the queue
    task_type pop_task()
    {
        std::unique_lock<std::mutex> lock{tasks_mutex_};
        cond_.wait(lock, [this]{return !tasks_.empty();});
        auto ret = tasks_.front();
        tasks_.pop_front();
        return ret;
    }

private:
    std::deque<task_type>   tasks_{};
    std::mutex              tasks_mutex_{};
    std::condition_variable cond_{};
};


auto g_queue = work_queue{};
constexpr auto tests_tasks_count = 300;

void do_nothing() { }

void pusher()
{
    for(int i = 0; i < tests_tasks_count; ++i){
        g_queue.push_task(&do_nothing);
    }
}

void popper_sync()
{
    for(std::size_t i = 0; i < tests_tasks_count; ++i){
        g_queue.pop_task()();   // get and execute a task;
    }
}

int main()
{
    auto pop_sync1 = std::thread{popper_sync};
    auto pop_sync2 = std::thread{popper_sync};
    auto pop_sync3 = std::thread{popper_sync};

    auto push1 = std::thread{pusher};
    auto push2 = std::thread{pusher};
    auto push3 = std::thread{pusher};

    pop_sync1.join();
    pop_sync2.join();
    pop_sync3.join();

    push1.join();
    push2.join();
    push3.join();

    assert(!g_queue.try_pop_task());

    g_queue.push_task(&do_nothing);
    assert(g_queue.try_pop_task());
}
