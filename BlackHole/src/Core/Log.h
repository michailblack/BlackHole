#pragma once

#include <memory>

#include "spdlog/spdlog.h"

class Log
{
public:
    static void Init();

    static void SetLogLevel(spdlog::level::level_enum level);

    inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

#ifndef NDEBUG
    #define BH_TRACE(...)       ::Log::GetLogger()->trace(__VA_ARGS__)
    #define BH_DEBUG(...)       ::Log::GetLogger()->debug(__VA_ARGS__)
    #define BH_INFO(...)        ::Log::GetLogger()->info(__VA_ARGS__)
    #define BH_WARN(...)        ::Log::GetLogger()->warn(__VA_ARGS__)
    #define BH_ERROR(...)       ::Log::GetLogger()->error(__VA_ARGS__)
    #define BH_CRITICAL(...)    ::Log::GetLogger()->critical(__VA_ARGS__)
#else
    #define BH_TRACE(...)
    #define BH_DEBUG(...)
    #define BH_INFO(...)
    #define BH_WARN(...)
    #define BH_ERROR(...)
    #define BH_CRITICAL(...)
#endif