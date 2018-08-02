#include <iostream>
#include <thread>



int main()
{
    std::cout << "Hello from thread[" << std::this_thread::get_id() << "]\n";
    auto t = std::thread{[]{
        std::cout << "Hello from thread[" << std::this_thread::get_id()
            << "]\n";
        }};
    t.join();
}
