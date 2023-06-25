#pragma once

#include <string>

#include "Core.h"
#include "Events/Event.h"

#include "GLFW/glfw3.h"

struct WindowProps
{
    std::string Title;
    unsigned int Width, Height;

    WindowProps(const std::string& title = "Black Hole",
                unsigned int width = 1280,
                unsigned int height = 720)
        : Title(title), Width(width), Height(height) {}
};

class Window
{
public:
    Window(const WindowProps& props = WindowProps());
    ~Window();

    void OnUpdate();

    inline unsigned int GetWidth() const { return m_Data.Width; }
    inline unsigned int GetHeight() const { return m_Data.Height; }

    void SetVSync(bool enabled);
    inline bool IsVSync() const { return m_Data.VSync; }
private:
    void Init(const WindowProps& props);
    void ShutDown();
private:
    GLFWwindow* m_Window;

    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;
    } m_Data;
};