#pragma once
#include "BlackHole/Events/Event.h"

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : m_MouseX(x), m_MouseY(y) {}

    float GetX() const { return m_MouseX; }
    float GetY() const { return m_MouseY; }
    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
        return oss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
private:
    float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset) {}

    float GetXOffset() const { return m_XOffset; }
    float GetYOffset() const { return m_YOffset; }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
        return oss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
private:
    float m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event
{
public:
    int GetMouseButton() const { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouseButton)
protected:
    MouseButtonEvent(int button)
        : m_Button(button) {}

    int m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button)
        : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseButtonPressedEvent: " << m_Button;
        return oss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button)
        : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseButtonReleasedEvent: " << m_Button;
        return oss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};