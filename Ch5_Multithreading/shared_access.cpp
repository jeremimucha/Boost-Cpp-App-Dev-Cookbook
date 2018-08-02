#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <shared_mutex>


struct user_info
{
    std::string address;
    unsigned short age;
};

class users_online
{
public:
    using mutex_t = std::shared_mutex;

    bool is_online(const std::string& username) const
    {
        std::shared_lock<mutex_t> lk{users_mutex_};
        return users_.find(username) != users_.end();
    }

    unsigned short get_age(const std::string& username) const
    {
        std::shared_lock<mutex_t> lk{users_mutex_};
        return users_.at(username).age;
    }

    void set_online(const std::string& username, const user_info& data)
    {
        std::lock_guard<mutex_t> lk{users_mutex_};
        users_.emplace(username, data);
    }

private:
    std::unordered_map<std::string, user_info> users_;
    mutable mutex_t                            users_mutex_;
};


int main()
{

}
