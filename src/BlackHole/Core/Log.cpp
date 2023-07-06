#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_Logger = nullptr;

void Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    spdlog::set_level(spdlog::level::trace);

    s_Logger = spdlog::stdout_color_mt("BLACK HOLE");
}

void Log::SetLogLevel(const spdlog::level::level_enum level)
{
    spdlog::set_level(level);
}
