#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>

namespace utilities
{

using long_int = long long;

class profiler
{
public:
    struct result
    {
        std::string name;
        long_int start_time_us;
        long_int end_time_us;
    };

    static profiler& instance()
    {
        static profiler _instance;
        return _instance;
    }

    void begin_session(const std::string &name, const std::string &filepath = "profiler_results.json")
    {
        m_output_stream.open(filepath);
        (void) name;
        write_header();
    }

    void end_session()
    {
        write_footer();
        m_output_stream.close();
        m_count = 0;
    }

    void write_profile(const profiler::result& res)
    {
        std::lock_guard<std::mutex> lk(m_mtx);
        // Add a comma between records
        if (m_count++ > 0)
        {
            m_output_stream << ",";
        }

        m_output_stream << "{";
        m_output_stream << "\"cat\":\"function\",";
        m_output_stream << "\"dur\":" << (res.end_time_us - res.start_time_us) << ",";
        m_output_stream << "\"name\":\"" << res.name << "\",";
        m_output_stream << "\"ph\":\"X\",";
        m_output_stream << "\"pid\":0,";
        m_output_stream << "\"tid\":" << std::this_thread::get_id() << ",";
        m_output_stream << "\"ts\":" << res.start_time_us;
        m_output_stream << "}";
        m_output_stream.flush();
    }

private:
    // Only create within the class
    profiler() = default;

    void write_header()
    {
        m_output_stream << "{\"otherData\": {},\"traceEvents\":[";
        m_output_stream.flush();
    }

    void write_footer()
    {
        m_output_stream << "]}";
        m_output_stream.flush();
    }

    std::ofstream m_output_stream;
    int m_count;
    std::mutex m_mtx;
};

class profiling_timer
{
public:
    profiling_timer(const std::string name) : m_name(name){}
    ~profiling_timer(){ if (m_running){stop();} }

    void stop()
    {
        auto end_tp = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_tp).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_tp).time_since_epoch().count();
        //std::cout << m_name << " : " << (end - start) << "us\n";
        profiler::instance().write_profile({m_name, start, end});
        m_running = false;
    }

private:
    std::string m_name{""};
    bool m_running{true};
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_tp{std::chrono::high_resolution_clock::now()};
};

#define PROFILING 1
#if PROFILING
    #define PROFILE_SCOPE(name) utilities::profiling_timer tmr##__LINE__(name)
#else
    #define PROFILE_SCOPE(name)
#endif
#define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)

} // utilities