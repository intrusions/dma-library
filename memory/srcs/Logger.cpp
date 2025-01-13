#include "Logger.hpp"

Logger logger;

std::string Logger::get_current_time() const
{
    std::ostringstream oss;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

constexpr const char* Logger::level_to_string(log_level level) const noexcept
{
    switch (level) {
        case log_level::info:       return "[*]";
        case log_level::warning:    return "[!]";
        case log_level::error:      return "[X]";
        default:                    return "[UNKNOWN]";
    }
}

void Logger::log(log_level level, const std::string& message) const
{
    if (is_debug) {
        const auto time_stamp = get_current_time();
        const auto log_message =    "[" + time_stamp + "]"
                                    + level_to_string(level)+ " "
                                    + message;
        
        if (level == log_level::error) {
            std::cerr << log_message << std::endl;
        } else {
            std::cout << log_message << std::endl;
        }
    }
}
