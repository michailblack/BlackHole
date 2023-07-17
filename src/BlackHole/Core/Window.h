#pragma once
#include <string>

#include "BlackHole/Events/Event.h"
#include "BlackHole/OpenGL/Context.h"

struct WindowProps
{
    std::string Title;
    unsigned int Width, Height;

    explicit WindowProps(const std::string& title = "Black Hole",
                         const unsigned int width = 1280,
                         const unsigned int height = 720)
        : Title(title), Width(width), Height(height) {}
};

class Window
{
    using EventCallbackFn = std::function<void(Event&)>;
public:
    explicit Window(const WindowProps& props = WindowProps());
    ~Window();

    void OnUpdate();

    unsigned int GetWidth() const { return m_Data.Width; }
    unsigned int GetHeight() const { return m_Data.Height; }

    void SetVSync(bool enabled);
    bool IsVSync() const { return m_Data.VSync; }
    void SetFullscreen(bool isFullscreen) { m_Data.IsFullscreen = isFullscreen; }
    bool IsFullscreen() const { return m_Data.IsFullscreen; }

    void SetCallbackFunction(const EventCallbackFn& eventCallback) { m_Data.EventCallback = eventCallback; }

    GLFWwindow* GetWindowGLFW() const { return m_Window; }
private:
    void Init(const WindowProps& props);
    void ShutDown();
private:
    GLFWwindow* m_Window;
    Scope<Context> m_Context;

    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;
        bool IsFullscreen;

        EventCallbackFn EventCallback;
    } m_Data;
};