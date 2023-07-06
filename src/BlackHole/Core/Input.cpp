#include "Input.h"
#include "Application.h"
#include "GLFW/glfw3.h"

bool Input::IsKeyPressed(int keyCode)
{
    auto* window = Application::Get().GetNativeWindow();
    const int state = glfwGetKey(window, keyCode);
    return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int buttonCode)
{
    auto* window = Application::Get().GetNativeWindow();
    const int state = glfwGetMouseButton(window, buttonCode);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
    auto* window = Application::Get().GetNativeWindow();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return { static_cast<float>(width), static_cast<float>(height) };
}

float Input::GetMouseX()
{
    return GetMousePosition().x;
}

float Input::GetMouseY()
{
    return GetMousePosition().y;
}