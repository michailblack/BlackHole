#pragma once
#include "Event.h"

class KeyEvent : public Event
{
public:
    int GetKeyCode() const { return m_KeyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
protected:
    KeyEvent(int keycode)
        : m_KeyCode(keycode) {}

    int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keycode, unsigned int repeatCount)
    : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

    unsigned int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
        return oss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    unsigned int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int keycode)
        : KeyEvent(keycode) {}

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "KeyReleasedEvent: " << m_KeyCode;
        return oss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};