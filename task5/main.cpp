#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <unistd.h>
#include <map>

template<class Derived>
class singleton {
public:
    static Derived *instance() {
        if (!m_ptr)
            m_ptr = new Derived();
        return m_ptr;
    }

    //attention: no destructor for singleton
    static void destroy() {
        delete m_ptr;
        m_ptr = nullptr;
    }

private:
    static Derived *m_ptr;
};

template<class Derived>
Derived *singleton<Derived>::m_ptr = nullptr;

// поддерживать 3 уровня важности событий (нормальный, замечание, ошибка);
enum log_type {
    LOG_NORMAL,
    LOG_WARNING,
    LOG_ERROR
};

class Logger : public singleton<Logger> {
    friend singleton<Logger>;
public:
    // обеспечить фиксацию события (с событием фиксируются время, важность, текстовое сообщение);
    void message(enum log_type type, std::string const &msg) {
        const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        rec.push_back(Record{type, time, msg});
        if (rec.size() > rec_limit) {
            rec.erase(rec.begin());
        }
    }

    // выводить на печать 10 последних событий.
    void print() {
        for (auto i: rec) {
            std::cout << text_type[i.type] << "\t"
                      << std::ctime(&i.time) << "\t"
                      << i.msg << std::endl;
        }
    }

private:
    //constructor must be private in order to forbid call without instance()
    Logger() {}

    struct Record {
        enum log_type type;
        std::time_t time;
        std::string msg;
    };

    std::vector<Record> rec;
    std::size_t rec_limit = 10;

    std::map<enum log_type, std::string> text_type{
            std::pair{log_type::LOG_NORMAL, "normal"},
            std::pair{log_type::LOG_WARNING, "warning"},
            std::pair{log_type::LOG_ERROR, "error"}};
};


int main() {
    Logger *log = Logger::instance();
    log->message(LOG_NORMAL, "Hello, world!");
    log->message(LOG_NORMAL, "123");

    sleep(1);

    log->message(LOG_NORMAL, "program loaded");
    log->message(LOG_WARNING, "warning! pay attention");
    log->message(LOG_ERROR, "error happens! help me!");
    log->print();

    Logger::destroy();
    return 0;
}
