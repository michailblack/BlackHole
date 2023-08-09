#pragma once
#include <filesystem>

#include "BlackHole/Core/LayerStack.h"
#include "BlackHole/Core/Timer.h"
#include "BlackHole/Core/Window.h"

#include "BlackHole/Events/ApplicationEvent.h"
#include "BlackHole/Events/KeyEvent.h"

#include "BlackHole/ImGui/ImGuiLayer.h"

struct ApplicationSpecification
{
    std::string Name = "Black Hole Application";
    std::filesystem::path WorkingDirectory;
};

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
protected:
    explicit Application(const ApplicationSpecification& specification);
private:

    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);
private:
    ApplicationSpecification m_Specification;
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

extern Application* CreateApplication();