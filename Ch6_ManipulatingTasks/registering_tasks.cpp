#include <iostream>
#include <type_traits>
#include <thread>
#include <chrono>
#include "task_processor.hpp"

using namespace std::literals::chrono_literals;

auto g_val = int{0};
void func_test()
{
    ++g_val;
    if (g_val == 3) {
        throw std::logic_error{"Just checking"};
    }

    // using std::thread so no interrupting threads...

    // if (g_val >= 50) {
    //     task_processor::get().stop();
    //     // on windows all the already posted tasks will execute anyway
    // }
}


int main()
{
    task_processor::get().run_after(boost::posix_time::seconds(1), []()noexcept{std::cout << "Hello from the past!\n";});
    // auto timer = make_timer_task(
    //     task_processor::get().ios_,
    //     boost::posix_time::seconds(1),
    //     []()noexcept{std::cout << "Hello from the past!\n";});
    // timer.push_task();
    // auto dt = boost::asio::deadline_timer{task_processor::get().ios_, boost::posix_time::seconds(1)};
    // dt.async_wait(
    //     [](const std::error_code& e)noexcept
    //     {
    //         if(!e)
    //             std::cerr << "Hello from the past!\n";
    //         else
    //             std::cerr << e << "\n";
    //     });

    constexpr auto task_count = int{100};
    static_assert(task_count > 90, "Task count must be > 90");
    for(int i = 0; i < task_count; ++i){
        task_processor::get().push_task(func_test);
    }

    // add a lambda
    task_processor::get().push_task(
        []()noexcept{ std::cout << "hello from thread[" << std::this_thread::get_id() << "]\n"; }
    );

    // will not throw, but blocks untill
    // one of the tasks calls stop()
    task_processor::get().start();
    std::cout << "\nDone, g_val == " << g_val;
    // task_processor::get().stop();
}
