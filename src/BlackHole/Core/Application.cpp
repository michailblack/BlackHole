#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application(const WindowProps& props)
{
    m_Window = std::make_unique<Window>(props);
    m_Window->SetCallbackFunction(BH_BIND_EVENT_FN(OnEvent));

    m_ImGuiLayer = std::make_unique<ImGuiLayer>();
    m_Camera = std::make_unique<PerspectiveCamera>(glm::radians(45.0f),
        static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight()),
        0.1f, 100.0f);
    m_CameraController = std::make_unique<PerspectiveCameraController>(static_cast<PerspectiveCamera*>(m_Camera.get()));

    float vertices[] = {
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    uint32_t indices[] = {
        0, 1, 3,
        1, 2 ,3,
        1, 5, 2,
        5, 6, 2,
        7, 4, 5,
        5, 6, 7,
        3, 0, 4,
        4, 7, 3,
        7, 3, 2,
        2, 6, 7,
        4, 0, 1,
        1, 5, 4
    };

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

    {
        const BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" }
        };

        m_VertexBuffer->SetLayout(layout);
    }

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_Shader = std::make_unique<Shader>("../../../assets/shaders/triangleShader.glsl");
}

Application::~Application()
{
    for (const auto layer : m_LayerStack)
        layer->OnDetach();
    delete s_Instance;
}

void Application::Init(const WindowProps& props)
{
    s_Instance = new Application(props);

    s_Instance->PushOverlay(s_Instance->GetImGuiLayer());
}

void Application::Run()
{
    while (m_IsRunning)
    {
        const float time = m_Timer.Elapsed();
        const Timestep ts = time - m_LastFrameTime;
        m_LastFrameTime = time;

        m_CameraController->OnUpdate(ts);

        Renderer::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
        Renderer::Clear();

        Renderer::BeginScene(m_Camera);
        Renderer::Submit(m_Shader, m_VertexArray);
        Renderer::EndScene();

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

    m_CameraController->OnEvent(e);

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.handled)
            break;
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