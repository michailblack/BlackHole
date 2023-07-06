#include "Window.h"

#include "BlackHole/Events/ApplicationEvent.h"
#include "BlackHole/Events/KeyEvent.h"
#include "BlackHole/Events/MouseEvent.h"

#include <GLFW/glfw3.h>

static bool gs_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
    BH_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

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
    m_Context->SwapBuffers();
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
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;

    BH_LOG_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

    if (!gs_GLFWInitialized)
    {
        int success = glfwInit();
        BH_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
        gs_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), props.title.c_str(), nullptr, nullptr);

    m_Context = std::make_unique<OpenGLContext>(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            
            data->width = width;
            data->height = height;

            WindowResizeEvent e(width, height);
            data->EventCallback(e);
        });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            WindowCloseEvent e;
            data->EventCallback(e);
        });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent e(key, 0);
                    data->EventCallback(e);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent e(key);
                    data->EventCallback(e);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent e(key, 1);
                    data->EventCallback(e);
                    break;
                }
                default:
                    BH_ASSERT(false, "Key action unknown!");
                    break;
            }
        });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent e(button);
                    data->EventCallback(e);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent e(button);
                    data->EventCallback(e);
                    break;
                }
                default:
                    BH_ASSERT(false, "Mouse button action unknown!");
                    break;
            }
        });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent e(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data->EventCallback(e);
        });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseMovedEvent e(static_cast<float>(xPos), static_cast<float>(yPos));
            data->EventCallback(e);
        });
}

void Window::ShutDown()
{
    glfwDestroyWindow(m_Window);
}
