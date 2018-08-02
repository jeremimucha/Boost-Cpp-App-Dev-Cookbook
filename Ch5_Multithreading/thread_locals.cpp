#include <iostream>
#include <thread>
#include <mutex>



class connection
{
public:
    void open()
    {
        std::cout << "thread[" << std::this_thread::get_id() << "]: opening connection\n";
    }

    void send_result(int result)
    {
        std::cout << "thread[" << std::this_thread::get_id() << "]: result = " << result << "\n";
    }
};


connection& get_connection()
{
    static thread_local connection con;
    static thread_local std::once_flag con_flag;
    std::call_once(con_flag, [&con=con]{con.open();});
    return con;
}

void task()
{
    const int result = []{
        std::cout << "thread[" << std::this_thread::get_id() << "]: computing result...\n";
        return 42;
        }();
    get_connection().send_result(result);
}

void run_tasks()
{
    for(int i = 0; i < 5; ++i){
        task();
    }
}


int main()
{
    auto t1 = std::thread{run_tasks};
    auto t2 = std::thread{run_tasks};
    t1.join();
    t2.join();
}
