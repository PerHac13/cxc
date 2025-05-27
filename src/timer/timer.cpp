#include "timer.hpp"

inline double Timer::elapsed_time()
{
    time_t acc_sec = time(0) - start_time;
    if (acc_sec < 3600)
        return (clock() - start_clock) / static_cast<double>(CLOCKS_PER_SEC);
    else
        return static_cast<double>(acc_sec);
}

inline double Timer::time_val()
{
    return elapsed_time();
}

inline void Timer::start(const char *msg)
{
    if (running)
        return;
    running = true;
    start_clock = clock();
    start_time = time(0);
    acc_time = 0;
    if (msg)
        std::cout << msg << std::endl;
}

inline void Timer::restart(const char *msg)
{
    if (running)
        stop();
    start(msg);
}

inline void Timer::stop(const char *msg)
{
    if (!running)
        return;
    running = false;
    acc_time += elapsed_time();
    if (msg)
        std::cout << msg << std::endl;
}

inline void Timer::check(const char *msg)
{
    if (msg)
        std::cout << msg << " : ";

    std::cout << "Elapsed time [" << std::setiosflags(std::ios::fixed)
              << std::setprecision(2)
              << acc_time + (running ? elapsed_time() : 0) << "] seconds\n";
}

inline std::ostream &operator<<(std::ostream &os, Timer &t)
{
    os << std::setprecision(2) << std::setiosflags(std::ios::fixed)
       << t.acc_time + (t.running ? t.elapsed_time() : 0);
    return os;
}