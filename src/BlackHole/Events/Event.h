#pragma once

#include <string>
#include <functional>

#include "BlackHole/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication    = BIT(0),
    EventCategoryInput          = BIT(1),
    EventCategoryKeyboard       = BIT(2),
    EventCategoryMouse          = BIT(3),
    EventCategoryMouseButton    = BIT(4)
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event
{
    friend class EventDispatcher;
public:
    bool handled = false;

    virtual EventType GetEventType() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual const char* GetName() const = 0;
    virtual std::string ToString() const { return GetName(); }

    bool IsInCategory(EventCategory category) const { return GetCategoryFlags() & category; }
};

template<typename OStream>
OStream& operator<<(OStream& os, const Event& e)
{
    fmt::format_to(std::ostream_iterator<char>(os), "{0}", e.ToString());
    return os;
};

class EventDispatcher
{
    template <typename T>
    using EventFn = std::function<bool(T&)>;
public:
    EventDispatcher(Event& e)
        : m_Event(e) {}

    template <typename T>
    bool Dispatch(EventFn<T> func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.handled |= func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};