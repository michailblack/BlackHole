#pragma once
#include "BlackHole/Core/Timestep.h"

#include "BlackHole/Events/Event.h"

class Layer
{
public:
    explicit Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    Layer(const Layer&) = delete;
    Layer(Layer&&) = delete;
    Layer& operator=(const Layer&) = delete;
    Layer& operator=(Layer&&) = delete;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& e) {}

    const std::string& GetName() const { return m_DebugName; }
protected:
    std::string m_DebugName;
};