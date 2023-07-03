#pragma once

struct GLFWwindow;

class OpenGLContext
{
public:
    OpenGLContext(GLFWwindow* window);

    void Init();
    void SwapBuffers();
private:
    GLFWwindow* m_WindowHandle;
};