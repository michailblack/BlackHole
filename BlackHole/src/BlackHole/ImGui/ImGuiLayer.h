#pragma once
#include "BlackHole/Core/Layer.h"

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& e) override;

    static void Begin();
    static void End();

    void BlockEvents(bool block) { m_BlockEvents = block; }

    void SetDarkThemeColors();
private:
    bool m_BlockEvents = true;
};