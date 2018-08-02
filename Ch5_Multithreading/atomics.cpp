#include <iostream>
#include <thread>
#include <atomic>


std::atomic<int> shared_i{0};

void do_inc()
{
    for(int i = 0; i < 30000; ++i){
        // do some work here
        // ...

        const int i_snapshot = ++shared_i;

        // do some work with i_snapshot;
        (void)i_snapshot;
    }
}

void do_dec()
{
    for(int i = 0; i < 30000; ++i){
        // do some work
        // ...

        const int i_snapshot = --shared_i;

        // do some work with i_snapshot;
        (void)i_snapshot;
    }
}


int main()
{
    auto t1 = std::thread{do_inc};
    auto t2 = std::thread{do_dec};
    t1.join();
    t2.join();

    std::cout << "shared_i == " << shared_i << std::endl;
}
