#include <iostream>
#include <thread>
#include <mutex>

std::mutex i_mutex;
int shared_i{0};

void do_inc()
{
    for(int i = 0; i < 30000; ++i){
        // do some work here
        // ...

        int i_snapshot;
        {
            std::lock_guard<std::mutex> lk{i_mutex};
            i_snapshot = ++shared_i;
        }

        // do some work with i_snapshot;
        (void)i_snapshot;
    }
}

void do_dec()
{
    for(int i = 0; i < 30000; ++i){
        // do some work
        // ...

        int i_snapshot;
        {
            std::lock_guard<std::mutex> lk{i_mutex};
            i_snapshot = --shared_i;
        }

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

    std::cout << "shared_i == " << shared_i << "\n";
}
