#include <iostream>
#include <thread>
#include <boost/thread.hpp>


boost::thread_group threads;


int main()
{
    for(int i = 0; i < 10; ++i){
        threads.create_thread([]{std::cout << "thread["
            << std::this_thread::get_id() << "] starting\n";
            });
    }
    threads.join_all();
}
