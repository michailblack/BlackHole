#include "bhpch.h"
#include "BlackHole/Core/Application.h"

#include "BlackHole/Renderer/Renderer.h"
#include "Platform/OpenGL/Framebuffer.h"

#include <GLFW/glfw3.h>

Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationSpecification& specification)
    : m_Specification(specification)
{
    BH_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    if (!specification.WorkingDirectory.empty())
        std::filesystem::current_path(specification.WorkingDirectory);

    WindowProps props;
    props.Title = specification.Name;

    m_Window = CreateScope<Window>(props);
    m_Window->SetCallbackFunction(BH_BIND_EVENT_FN(OnEvent));

    Renderer::Init();
    
    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application()
{
    for (const auto layer : m_LayerStack)
        layer->OnDetach();
}

void Application::Run()
{
    while (m_IsRunning)
    {
        const float time = m_Timer.Elapsed();
        const Timestep ts = time - m_LastFrameTime;
        m_LastFrameTime = time;

        Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f});
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
    dispatcher.Dispatch<WindowCloseEvent>(BH_BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BH_BIND_EVENT_FN(OnWindowResize));

    for (const auto& layer : m_LayerStack | std::views::reverse)
    {
        if (e.Handled)
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