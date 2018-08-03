#pragma once
#include <utility>
#include <system_error>
#include <memory>
#include <functional>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

template<typename T>
class task_wrapper
{
public:
    explicit constexpr task_wrapper(const T& task) noexcept(std::is_nothrow_constructible_v<T>)
        : task_{task}
        { }

    void operator()() const noexcept
    try{    // make sure we don't throw an exception
        task_();
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    } catch(...) {
        std::cerr << "Unknown exception\n";
    }

private:
    T task_;
};

template<typename T>
constexpr auto make_task_wrapper(T&& task) noexcept
{
    return task_wrapper<T>{std::forward<T>(task)};
}


using duration_type = boost::asio::deadline_timer::duration_type;
using time_type     = boost::asio::deadline_timer::time_type;
using sec_t         = boost::posix_time::seconds;

template<typename Function>
class timer_task : public task_wrapper<Function>
{
public:
    using deadline_timer_t = boost::asio::deadline_timer;
    using io_service_t    = boost::asio::io_service;

    template<typename Time>
    explicit timer_task(io_service_t& ios, Time&& time, Function&& task)
        : base{std::move(task)}
        , timer_{std::make_shared<deadline_timer_t>(std::ref(ios), std::forward<Time>(time))}
        {}

    void push_task() const
    {
        timer_->async_wait(*this);
    }

    void operator()(const std::error_code& error) const
    {
        if(!error){
            base::operator()();
        } else {
            std::cerr << error << "\n";
        }
    }
private:
    using base = task_wrapper<Function>;
    std::shared_ptr<boost::asio::deadline_timer> timer_;
};

template<typename Time, typename Function>
constexpr auto make_timer_task(boost::asio::io_service& ios, Time&& time, Function&& task) noexcept
{
    return timer_task<Function>{ios, std::forward<Time>(time), std::forward<Function>(task)};
}

class task_processor
{
public:

    ~task_processor() noexcept = default;

    task_processor(task_processor&&) noexcept = default;
    task_processor& operator=(task_processor&&) noexcept = default;

    task_processor(const task_processor&) = delete;
    task_processor& operator=(const task_processor&) = delete;

    static task_processor& get() noexcept
    {
        static auto instance = task_processor{};
        return instance;
    }

    template<typename T>
    inline void push_task(const T& task) noexcept
    {
        ios_.post(make_task_wrapper(task));
    }

    inline void start() { ios_.run(); }

    inline void stop() { ios_.stop(); }

    template<typename Time, typename Function>
    void run_at(Time&& time, Function&& f)
    {
        make_timer_task(ios_, std::forward<Time>(time), std::forward<Function>(f)).push_task();
    }

    template<typename Time, typename Function>
    void run_after(Time&& duration, Function&& f)
    {
        make_timer_task(ios_, std::forward<Time>(duration), std::forward<Function>(f)).push_task();
    }

// private:
    task_processor() = default;
    boost::asio::io_service         ios_{};
    boost::asio::io_service::work   work_{ios_};
};