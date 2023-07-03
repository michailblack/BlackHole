#pragma once

#include <string>

#include "BlackHole/Events/Event.h"
#include "BlackHole/Renderer/OpenGLContext.h"

struct WindowProps
{
    std::string title;
    unsigned int width, height;

    explicit WindowProps(const std::string& t_title = "Black Hole",
                         const unsigned int t_width = 1280,
                         const unsigned int t_height = 720)
        : title(t_title), width(t_width), height(t_height) {}
};

class Window
{
    using EventCallbackFn = std::function<void(Event&)>;
public:
    explicit Window(const WindowProps& props = WindowProps());
    ~Window();

    void OnUpdate();

    unsigned int GetWidth() const { return m_Data.width; }
    unsigned int GetHeight() const { return m_Data.height; }

    void SetCallbackFunction(const EventCallbackFn& eventCallback) { m_Data.EventCallback = eventCallback; }

    void SetVSync(bool enabled);
    bool IsVSync() const { return m_Data.VSync; }
    GLFWwindow* GetWindowGLFW() const { return m_Window; };

private:
    void Init(const WindowProps& props);
    void ShutDown();
private:
    GLFWwindow* m_Window;
    std::unique_ptr<OpenGLContext> m_Context;

    struct WindowData
    {
        std::string title;
        unsigned int width, height;
        bool VSync;

        EventCallbackFn EventCallback;
    } m_Data;
};