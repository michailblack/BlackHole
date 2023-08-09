#include "bhpch.h"
#include "BlackHole/Core/Input.h"
#include "BlackHole/Core/Application.h"

#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(int keyCode)
{
    auto* window = Application::Get().GetWindow().GetNativeWindow();
    const int state = glfwGetKey(window, keyCode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int buttonCode)
{
    auto* window = Application::Get().GetWindow().GetNativeWindow();
    const int state = glfwGetMouseButton(window, buttonCode);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
    auto* window = Application::Get().GetWindow().GetNativeWindow();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return { width, height };
}

float Input::GetMouseX()
{
    return GetMousePosition().x;
}

float Input::GetMouseY()
{
    return GetMousePosition().y;
}