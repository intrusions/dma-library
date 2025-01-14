#pragma once

#include <string>
#include <iostream>
#include <fmt/core.h>
#include <fmt/chrono.h>

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
    constexpr const char* level_to_string(log_level level) const noexcept
    {
        switch (level) {
            case log_level::info:       return "[*]";
            case log_level::warning:    return "[!]";
            case log_level::error:      return "[X]";
            default:                    return "[UNKNOWN]";
        }
    }

public:
    template <typename... Args>
    void log(log_level level, const std::string& fmt, Args&&... args) const
    {
        if (is_debug) {
            const auto log_message = fmt::format("[{:%H:%M:%S}] {} {}", 
                                                std::chrono::system_clock::now(),
                                                level_to_string(level), 
                                                fmt::format(fmt, std::forward<Args>(args)...));

            if (level == log_level::error) {
                std::cerr << log_message << std::endl;
            } else {
                std::cout << log_message << std::endl;
            }
        }
    }
};

extern Logger logger;
