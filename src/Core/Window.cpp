#include "bhpch.h"
#include "Window.h"

static bool s_GLFWInitialized = false;

Window::Window(const WindowProps& props)
{
    Init(props);
}

Window::~Window()
{
    ShutDown();
}

void Window::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void Window::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_Data.VSync = enabled;
}

void Window::Init(const WindowProps& props)
{
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    BH_LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized)
    {
        int success = glfwInit();
        BH_ASSERT(success, "Could not initialize GLFW!");

        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);
}

void Window::ShutDown()
{
    glfwDestroyWindow(m_Window);
}
