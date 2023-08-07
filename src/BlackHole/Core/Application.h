#pragma once
#include "LayerStack.h"
#include "Timer.h"
#include "Window.h"

#include "BlackHole/Events/ApplicationEvent.h"
#include "BlackHole/Events/KeyEvent.h"

#include "BlackHole/ImGui/ImGuiLayer.h"

class Application
{
public:
    ~Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    void Run();
    void Close() { m_IsRunning = false; }

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    static Application& Get() { return *s_Instance; }
    Window& GetWindow() const { return *m_Window; }
    ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
private:
    explicit Application(const WindowProps& props);

    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
private:
    bool m_IsRunning = true;
    Scope<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    LayerStack m_LayerStack;
    Timer m_Timer;
    float m_LastFrameTime = 0.0f;
private:
    static Application* s_Instance;
    friend int main(int argc, char** argv);
};
