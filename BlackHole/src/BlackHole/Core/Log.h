#pragma once
#include <spdlog/spdlog.h>
#include <unordered_set>

class Log
{
public:
    static void Init();
    static void SetLogLevel(spdlog::level::level_enum level);
    static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
    static std::shared_ptr<std::unordered_set<std::string>>& GetLoggedMessages() { return s_LoggedMessages; }
private:
    static std::shared_ptr<spdlog::logger> s_Logger;
    static std::shared_ptr<std::unordered_set<std::string>> s_LoggedMessages;
};

#ifdef BH_DEBUG
    #define BH_GET_LINE_STR(x) BH_STRINGIFY_MACRO(x)

    #define BH_LOG_TRACE(...) \
            { \
                std::string key = __FILE__ BH_GET_LINE_STR(__LINE__) BH_STRINGIFY_MACRO(__VA_ARGS__); \
                if (::Log::GetLoggedMessages()->find(key) == ::Log::GetLoggedMessages()->end()) \
                { \
                    ::Log::GetLoggedMessages()->insert(key); \
                    ::Log::GetLogger()->trace(__VA_ARGS__); \
                } \
            }
    #define BH_LOG_DEBUG(...) \
                { \
                    std::string key = __FILE__ BH_GET_LINE_STR(__LINE__) BH_STRINGIFY_MACRO(__VA_ARGS__); \
                    if (::Log::GetLoggedMessages()->find(key) == ::Log::GetLoggedMessages()->end()) \
                    { \
                        ::Log::GetLoggedMessages()->insert(key); \
                        ::Log::GetLogger()->debug(__VA_ARGS__); \
                    } \
                }
    #define BH_LOG_INFO(...) \
                { \
                    std::string key = __FILE__ BH_GET_LINE_STR(__LINE__) BH_STRINGIFY_MACRO(__VA_ARGS__); \
                    if (::Log::GetLoggedMessages()->find(key) == ::Log::GetLoggedMessages()->end()) \
                    { \
                        ::Log::GetLoggedMessages()->insert(key); \
                        ::Log::GetLogger()->info(__VA_ARGS__); \
                    } \
                }
    #define BH_LOG_WARN(...) \
        { \
            std::string key = __FILE__ BH_GET_LINE_STR(__LINE__) BH_STRINGIFY_MACRO(__VA_ARGS__); \
            if (::Log::GetLoggedMessages()->find(key) == ::Log::GetLoggedMessages()->end()) \
            { \
                ::Log::GetLoggedMessages()->insert(key); \
                ::Log::GetLogger()->warn(__VA_ARGS__); \
            } \
        }
    #define BH_LOG_ERROR(...) \
            { \
                std::string key = __FILE__ BH_GET_LINE_STR(__LINE__) BH_STRINGIFY_MACRO(__VA_ARGS__); \
                if (::Log::GetLoggedMessages()->find(key) == ::Log::GetLoggedMessages()->end()) \
                { \
                    ::Log::GetLoggedMessages()->insert(key); \
                    ::Log::GetLogger()->error(__VA_ARGS__); \
                } \
            }
    #define BH_LOG_CRITICAL(...) \
            { \
                std::string key = __FILE__ BH_GET_LINE_STR(__LINE__) BH_STRINGIFY_MACRO(__VA_ARGS__); \
                if (::Log::GetLoggedMessages()->find(key) == ::Log::GetLoggedMessages()->end()) \
                { \
                    ::Log::GetLoggedMessages()->insert(key); \
                    ::Log::GetLogger()->critical(__VA_ARGS__); \
                } \
            }
#else
    #define BH_LOG_TRACE(...)
    #define BH_LOG_DEBUG(...)
    #define BH_LOG_INFO(...)
    #define BH_LOG_WARN(...)
    #define BH_LOG_ERROR(...)
    #define BH_LOG_CRITICAL(...)
#endif