#pragma once
#include "spdlog/spdlog.h"

class Log
{
public:
    static void Init();
    static void SetLogLevel(spdlog::level::level_enum level);
    static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

#ifdef BH_DEBUG
    #define BH_LOG_TRACE(...)       ::Log::GetLogger()->trace(__VA_ARGS__)
    #define BH_LOG_DEBUG(...)       ::Log::GetLogger()->debug(__VA_ARGS__)
    #define BH_LOG_INFO(...)        ::Log::GetLogger()->info(__VA_ARGS__)
    #define BH_LOG_WARN(...)        ::Log::GetLogger()->warn(__VA_ARGS__)
    #define BH_LOG_ERROR(...)       ::Log::GetLogger()->error(__VA_ARGS__)
    #define BH_LOG_CRITICAL(...)    ::Log::GetLogger()->critical(__VA_ARGS__)
#else
    #define BH_LOG_TRACE(...)
    #define BH_LOG_DEBUG(...)
    #define BH_LOG_INFO(...)
    #define BH_LOG_WARN(...)
    #define BH_LOG_ERROR(...)
    #define BH_LOG_CRITICAL(...)
#endif