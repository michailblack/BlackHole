#pragma once

struct GLFWwindow;

class Context
{
public:
    Context(GLFWwindow* window);

    void Init();
    void SwapBuffers();
private:
    GLFWwindow* m_WindowHandle;
};