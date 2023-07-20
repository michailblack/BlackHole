#include "Context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Context::Context(GLFWwindow* window)
    : m_WindowHandle(window)
{
}

void Context::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    BH_ASSERT(status, "Failed to initialize Glad!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_STENCIL_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    BH_LOG_INFO("[OpenGL] Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    BH_LOG_INFO("[OpenGL] Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    BH_LOG_INFO("[OpenGL] Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void Context::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);
}