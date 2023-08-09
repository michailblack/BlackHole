#pragma once
#include "BlackHole/Events/Event.h"
#include "Platform/OpenGL/Context.h"

struct WindowProps
{
    std::string Title;
    uint32_t Width, Height;

    explicit WindowProps(const std::string& title = "Black Hole Window",
                         const uint32_t width = 1280,
                         const uint32_t height = 720)
        : Title(title), Width(width), Height(height) {}
};

class Window
{
    using EventCallbackFn = std::function<void(Event&)>;
public:
    explicit Window(const WindowProps& props = WindowProps());
    ~Window();

    void OnUpdate();

    uint32_t GetWidth() const { return m_Data.Width; }
    uint32_t GetHeight() const { return m_Data.Height; }

    void SetVSync(bool enabled);
    bool IsVSync() const { return m_Data.VSync; }

    void SetFullscreen(bool isFullscreen);
    bool IsFullscreen() const { return m_Data.IsFullscreen; }

    void SetCallbackFunction(const EventCallbackFn& eventCallback) { m_Data.EventCallback = eventCallback; }

    GLFWwindow* GetNativeWindow() const { return m_Window; }
private:
    void Init(const WindowProps& props);
    void ShutDown();
private:
    GLFWwindow* m_Window;
    Scope<Context> m_Context;

    struct WindowData
    {
        std::string Title;
        uint32_t Width, Height;
        bool VSync;
        bool IsFullscreen;

        EventCallbackFn EventCallback;
    } m_Data;
};