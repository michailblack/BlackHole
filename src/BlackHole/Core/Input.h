#pragma once
#include <glm/vec2.hpp>

class Input
{
public:
    static bool IsKeyPressed(int keyCode);

    static bool IsMouseButtonPressed(int buttonCode);
    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};
