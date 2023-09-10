#include "bhpch.h"
#include "Platform/OpenGL/Context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void APIENTRY OpenGLErrorCallback(
    GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char *message,
    const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::string sourceStr;
    std::string typeStr;
    std::string msg;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             sourceStr += "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr += "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr += "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr += "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     sourceStr += "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           sourceStr += "Other"; break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               typeStr = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               typeStr = "Other"; break;
    }

    //msg = "[OpenGL] " + sourceStr + "; " + typeStr + "\nMessage: " + message;
    msg = "[OpenGL] " + typeStr + " in " + sourceStr + ":\n" + message;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         BH_ASSERT(false, msg); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       BH_LOG_ERROR(msg); break;
        case GL_DEBUG_SEVERITY_LOW:          BH_LOG_WARN(msg); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: BH_LOG_INFO(msg); break;
    }
}

Context::Context(GLFWwindow* window)
    : m_WindowHandle(window)
{
    BH_ASSERT(window, "Window handle is null!");
}

void Context::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    BH_ASSERT(status, "Failed to initialize Glad!");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_STENCIL_TEST);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_MULTISAMPLE);

    BH_LOG_INFO("[OpenGL] Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    BH_LOG_INFO("[OpenGL] Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    BH_LOG_INFO("[OpenGL] Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

#ifdef BH_DEBUG
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLErrorCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        BH_LOG_DEBUG("[OpenGL] Debug context: Initialized successfully.");
    }
#endif
}

void Context::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);
}