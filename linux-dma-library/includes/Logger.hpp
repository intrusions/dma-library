#pragma once

#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

constexpr bool is_debug = 
#ifdef DEBUG
    true;
#else
    false;
#endif

enum class log_level
{
    info,
    warning,
    error
};

class Logger
{
private:
    std::string get_current_time() const;
    constexpr const char* level_to_string(log_level level) const noexcept;

public:
    void log(log_level level, const std::string& message) const;

};

extern Logger logger;
