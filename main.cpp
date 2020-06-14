#include <iostream>
#include <vector>
#include "profiling.hpp"

template<typename T>
void calc(size_t num = 1000000)
{
    PROFILE_FUNCTION();
    T value = 1;
    for (T i = 1; i < (T) num; i++)
    {
        value = value * i;
        value = value / i;
        value++;
    }
}

int main()
{
    utilities::profiler::instance().begin_session("s1");

    auto work = []{
        PROFILE_FUNCTION();
        calc<int>();
        calc<uint64_t>();
        calc<float>();
        calc<double>();
        calc<int>();
    };

    {
        PROFILE_FUNCTION();

        std::vector<std::thread> threads;
        for (int i = 0; i < 5; i++)
        {
            threads.emplace_back(std::thread(work));
        }

        for (auto &t : threads)
        {
            if (t.joinable()) {t.join();}
        }
    }
    utilities::profiler::instance().end_session();
    return 0;
}