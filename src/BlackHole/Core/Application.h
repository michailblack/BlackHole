#pragma once

#include "LayerStack.h"
#include "Timer.h"
#include "Window.h"

#include "BlackHole/Events/ApplicationEvent.h"
#include "BlackHole/Events/Event.h"

#include "BlackHole/Renderer/Buffer.h"
#include "BlackHole/Renderer/Camera.h"
#include "BlackHole/Renderer/CameraController.h"
#include "BlackHole/Renderer/Shader.h"

#include "BlackHole/ImGui/ImGuiLayer.h"

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
    ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer.get(); }

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    Window& GetWindow() const { return *m_Window; }
    GLFWwindow* GetNativeWindow() const { return m_Window->GetWindowGLFW(); }
public:
    Ref<VertexArray> m_VertexArray;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<IndexBuffer> m_IndexBuffer;
    Ref<Shader> m_Shader;

private:
    explicit Application(const WindowProps& props);

    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
private:
    static Application* s_Instance;

    bool m_IsRunning = true;
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
    LayerStack m_LayerStack;
    Timer m_Timer;
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<CameraController> m_CameraController;
    float m_LastFrameTime = 0.0f;
};