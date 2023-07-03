#include "bhpch.h"
#include "Application.h"

#include <glad/glad.h>

#include "BlackHole/ImGui/ImGuiLayer.h"

Application* Application::s_Instance = nullptr;

Application::Application(const WindowProps& props)
{
    m_Window = std::make_unique<Window>(props);
    m_Window->SetCallbackFunction(BH_BIND_EVENT_FN(OnEvent));

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  0.8f, 0.2f, 0.8f, 1.0f,
         0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.8f, 1.0f,
         0.0f,  0.5f, 0.0f,  0.8f, 0.8f, 0.2f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 2
    };

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

    {
        const BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
        };

        m_VertexBuffer->SetLayout(layout);
    }

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    std::string vertexSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 a_Position;
        layout (location = 1) in vec4 a_Color;
        
        out vec4 v_Color;

        void main()
        {
            v_Color = a_Color;
            gl_Position = vec4(a_Position, 1.0);
        }
    )";

    std::string fragmentSrc = R"(
        #version 330 core
        layout (location = 0) out vec4 FragColor;        

        in vec4 v_Color;

        void main()
        {
            FragColor = v_Color;
        }
    )";

    m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
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

    s_Instance->PushOverlay(new ImGuiLayer());
}

void Application::Run()
{
    while (m_IsRunning)
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Shader->Bind();
        m_VertexArray->Bind();
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

        for (Layer* layer : m_LayerStack)
            layer->OnUpdate();

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

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.handled)
            break;
    }

    BH_LOG_TRACE(e);
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