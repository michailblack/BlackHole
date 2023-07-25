#include "Application.h"

#include <GLFW/glfw3.h>

#include "BlackHole/Layers/WorldLayer.h"

Application* Application::s_Instance = nullptr;

Application::Application(const WindowProps& props)
{
    m_Window = CreateScope<Window>(props);
    m_Window->SetCallbackFunction(BH_BIND_EVENT_FN(OnEvent));

    m_ImGuiLayer = CreateScope<ImGuiLayer>();
}

Application::~Application()
{
    for (const auto layer : m_LayerStack)
        layer->OnDetach();
    delete s_Instance;
}

void Application::Init()
{
    s_Instance = new Application(WindowProps());
    s_Instance->PushOverlay(s_Instance->GetImGuiLayer());

    Renderer::Init();

    s_Instance->PushLayer(new WorldLayer());
}

void Application::Run()
{
    while (m_IsRunning)
    {
        const float time = m_Timer.Elapsed();
        const Timestep ts = time - m_LastFrameTime;
        m_LastFrameTime = time;

        Renderer::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
        Renderer::Clear();

        for (Layer* layer : m_LayerStack)
            layer->OnUpdate(ts);

        ImGuiLayer::Begin();
        for (Layer* layer : m_LayerStack)
            layer->OnImGuiRender();
        ImGuiLayer::End();

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<KeyPressedEvent>(BH_BIND_EVENT_FN(OnKeyPressed));
    dispatcher.Dispatch<WindowCloseEvent>(BH_BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BH_BIND_EVENT_FN(OnWindowResize));

    for (const auto layer : m_LayerStack | std::views::reverse)
    {
        if (e.handled)
            break;
        layer->OnEvent(e);
    }
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_IsRunning = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
    Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
    return true;
}

bool Application::OnKeyPressed(KeyPressedEvent& e)
{
    switch (e.GetKeyCode())
    {
    case GLFW_KEY_ESCAPE:
        m_IsRunning = false;
        break;
    case GLFW_KEY_F11:
        m_Window->SetFullscreen(!m_Window->IsFullscreen());
        break;
    }

    return true;
}