#include "Application.h"

#include <GLFW/glfw3.h>

Application* Application::s_Instance = nullptr;

Application::Application(const WindowProps& props)
{
    m_Window = CreateScope<Window>(props);
    m_Window->SetCallbackFunction(BH_BIND_EVENT_FN(OnEvent));

    FramebufferSpecification fbSpec;
    fbSpec.Width = m_Window->GetWidth();
    fbSpec.Height = m_Window->GetHeight();
    m_Framebuffer = CreateScope<Framebuffer>(fbSpec);

    m_ImGuiLayer = CreateScope<ImGuiLayer>();
    m_Camera = CreateScope<PerspectiveCamera>(45.0f,
        static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight()),
        0.1f, 100.0f);
    m_CameraController = CreateScope<PerspectiveCameraController>(static_cast<PerspectiveCamera*>(m_Camera.get()));

    m_WindowVAO = CreateRef<VertexArray>();
    float windowVertices[] = {
        -0.5f, -0.5f, 0.0f,    0.0f,  0.0f,
         0.5f, -0.5f, 0.0f,    1.0f,  0.0f,
         0.5f,  0.5f, 0.0f,    1.0f,  1.0f,
        -0.5f,  0.5f, 0.0f,    0.0f,  1.0f
    };

    uint32_t windowIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    m_WindowVBO = CreateRef<VertexBuffer>(windowVertices, sizeof(windowVertices));
    m_WindowVBO->SetLayout({
        { ShaderDataType::Float3, "a_Position"      },
        { ShaderDataType::Float2, "a_TextureCoords" }
    });

    m_WindowIBO = CreateRef<IndexBuffer>(windowIndices, sizeof(windowIndices) / sizeof(uint32_t));

    m_WindowVAO->AddVertexBuffer(m_WindowVBO);
    m_WindowVAO->SetIndexBuffer(m_WindowIBO);

    m_ScreenSquadVAO = CreateRef<VertexArray>();
    float screenSquadVertices[] = {
        -1.0f, -1.0f,    0.0f, 0.0f,
         1.0f, -1.0f,    1.0f, 0.0f,
         1.0f,  1.0f,    1.0f, 1.0f,
        -1.0f,  1.0f,    0.0f, 1.0f,
    };

    uint32_t screenSquadIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    m_ScreenSquadVBO = CreateRef<VertexBuffer>(screenSquadVertices, sizeof(screenSquadVertices));
    m_ScreenSquadVBO->SetLayout({
        { ShaderDataType::Float2, "a_Position"      },
        { ShaderDataType::Float2, "a_TextureCoords" }
    });

    m_ScreenSquadIBO = CreateRef<IndexBuffer>(screenSquadIndices, sizeof(screenSquadIndices) / sizeof(uint32_t));

    m_ScreenSquadVAO->AddVertexBuffer(m_ScreenSquadVBO);
    m_ScreenSquadVAO->SetIndexBuffer(m_ScreenSquadIBO);

    m_ShaderLibrary.Load("../../../assets/shaders/transparentCheck.glsl");
    m_ShaderLibrary.Load("../../../assets/shaders/screenSquad.glsl");
    m_ShaderLibrary.Load("../../../assets/shaders/model.glsl");

    m_Model = CreateRef<Model>("../../../assets/models/BarberShopChair_01_8k/BarberShopChair_01_8k.fbx");
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

        m_Framebuffer->Bind();
        glEnable(GL_DEPTH_TEST);
        Renderer::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
        Renderer::Clear();

        Renderer::BeginScene(m_Camera);
        Renderer::Submit(m_ShaderLibrary.Get("model"), m_Model);
        Renderer::Submit(m_ShaderLibrary.Get("transparentCheck"), m_WindowVAO);
        Renderer::EndScene();

        // Temporary for testing Framebuffer class
        Framebuffer::Unbind();
        Renderer::SetClearColor(glm::vec4(1.0f));
        Renderer::Clear();
        m_ShaderLibrary.Get("screenSquad")->Bind();
        m_ScreenSquadVAO->Bind();
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, m_Framebuffer->GetColorAttachmentRendererID());
        glDrawElements(GL_TRIANGLES, m_ScreenSquadVAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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
    m_Framebuffer->Resize(m_Window->GetWidth(), m_Window->GetHeight());
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
        {
            const int monitorWidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
            const int monitorHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
            if (m_Window->IsFullscreen())
            {
                glfwSetWindowMonitor(m_Window->GetWindowGLFW(),
                    nullptr, 
                    monitorWidth / 4, monitorHeight / 4,
                    monitorWidth / 2, monitorHeight / 2,
                    GLFW_DONT_CARE);
                m_Window->SetFullscreen(false);
            }
            else
            {
                glfwSetWindowMonitor(m_Window->GetWindowGLFW(),
                    glfwGetPrimaryMonitor(), 
                    0, 0,
                    monitorWidth, monitorHeight,
                    GLFW_DONT_CARE);
                m_Window->SetFullscreen(true);
            }
            break;
        }
    }

    return true;
}