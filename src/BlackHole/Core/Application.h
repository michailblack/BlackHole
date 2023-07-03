#pragma once

#include "LayerStack.h"
#include "Window.h"

#include "BlackHole/Events/ApplicationEvent.h"
#include "BlackHole/Events/Event.h"

#include "BlackHole/Renderer/Buffer.h"
#include "BlackHole/Renderer/Shader.h"

class Application
{
public:
    ~Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;


    static void Init(const WindowProps& props = WindowProps());
    static Application& Get() { return *s_Instance; }

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    Window& GetWindow() const { return *m_Window; }
    GLFWwindow& GetNativeWindow() const { return *m_Window->GetWindowGLFW(); }

    std::shared_ptr<VertexArray> m_VertexArray;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<Shader> m_Shader;

private:
    Application(const WindowProps& props);

    bool OnWindowClose(WindowCloseEvent& e);

    static Application* s_Instance;

    std::unique_ptr<Window> m_Window;

    bool m_IsRunning = true;
    LayerStack m_LayerStack;
};
