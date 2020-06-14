#include <iostream>
#include "profiling.hpp"

int main()
{
    utilities::profiler::instance().begin_session("s1");

    {
        utilities::profiling_timer tmr("main");
        for (int i = 0; i < 1000000; i++) {}
    }

    utilities::profiler::instance().end_session();
    return 0;
}