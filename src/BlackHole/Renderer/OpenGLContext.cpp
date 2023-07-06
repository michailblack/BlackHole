#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

OpenGLContext::OpenGLContext(GLFWwindow* window)
    : m_WindowHandle(window)
{
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    BH_ASSERT(status, "Failed to initialize Glad!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    BH_LOG_INFO("[OpenGL] Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    BH_LOG_INFO("[OpenGL] Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    BH_LOG_INFO("[OpenGL] Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);
}