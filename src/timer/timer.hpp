#pragma once

#include <ctime>
#include <iostream>
#include <iomanip>

class Timer
{
    friend std::ostream &operator<<(std::ostream &os, Timer &t);

private:
    bool running;
    clock_t start_clock;
    time_t start_time;
    double acc_time;

    double elapsed_time();

public:
    Timer();

    void start(const char *msg = 0);
    void restart(const char *msg = 0);
    void stop(const char *msg = 0);
    void check(const char *msg = 0);
    double time_val();
};