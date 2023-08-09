#pragma once
#include <memory>

#include "BlackHole/Core/Log.h"

#ifdef BH_ENABLE_ASSERTS
    #define BH_ASSERT(x, ...) { if(!(x)) { BH_LOG_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define BH_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BH_BIND_EVENT_FN(EventFn) [this]<typename EventType>(EventType&& e) { return this->EventFn(std::forward<EventType>(e)); }

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}