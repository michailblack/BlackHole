#include "bhpch.h"
#include "Application.h"

Application::Application()
{
    Log::Init();
    m_Window = std::unique_ptr<Window>(new Window());
}

Application::~Application()
{
}

void Application::Run()
{
    while (m_Running)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        m_Window->OnUpdate();
    }
}
