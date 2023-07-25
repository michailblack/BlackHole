#pragma once

#include "LayerStack.h"
#include "Timer.h"
#include "Window.h"

#include "BlackHole/Events/Event.h"
#include "BlackHole/Events/ApplicationEvent.h"
#include "BlackHole/Events/KeyEvent.h"

#include "BlackHole/OpenGL/Shader.h"

#include "BlackHole/ImGui/ImGuiLayer.h"

class Application
{
public:
    ~Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    static void Init();
    static Application& Get() { return *s_Instance; }
    ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer.get(); }

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    Window& GetWindow() const { return *m_Window; }
    GLFWwindow* GetNativeWindow() const { return m_Window->GetWindowGLFW(); }
private:
    explicit Application(const WindowProps& props);

    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
private:
    static Application* s_Instance;

    bool m_IsRunning = true;
    Scope<Window> m_Window;
    Scope<ImGuiLayer> m_ImGuiLayer;
    LayerStack m_LayerStack;
    Timer m_Timer;
    float m_LastFrameTime = 0.0f;
};