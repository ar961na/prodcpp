#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <thread>

enum LogLevel {
    LOG_NORMAL,
    LOG_WARNING,
    LOG_ERROR
};

class Log
{
    static constexpr int maxLogs = 10;
    static Log* m_Instance;

    using Clock = std::chrono::steady_clock;
    Clock::time_point m_Start;

    std::vector<std::tuple<LogLevel, Clock::time_point, std::string>> logs;

    Log()
            :	m_Start {Clock::now()}
    {}

public:
    static Log* Instance()
    {
        if (!m_Instance)
            m_Instance = new Log;
        return m_Instance;
    }

    void message(LogLevel level, const std::string& message)
    {
        logs.push_back(std::make_tuple(level, std::chrono::steady_clock::now(), message));

        if (logs.size() > maxLogs)
            logs.erase(logs.begin());
    }

    void print()
    {
        using namespace std::chrono;
        for (const auto& log : logs)
            std::cout << "[" << std::get<0>(log) << "] " << (duration_cast<milliseconds>(std::get<1>(log) - m_Start)).count() << ": " << std::get<2>(log) << "\n";
    }
};

Log* Log::m_Instance = nullptr;

int main()
{
    using namespace std::chrono;
    Log *log = Log::Instance();
    log->message(LOG_NORMAL, "program loaded");
    log->message(LOG_NORMAL, "point 1");
    std::this_thread::sleep_for(1ms);
    log->message(LOG_NORMAL, "point 2");
    std::this_thread::sleep_for(3ms);
    log->message(LOG_NORMAL, "point 3");
    std::this_thread::sleep_for(1s);
    log->message(LOG_ERROR, "error happens! help me!");
    log->print();
    return 0;
}