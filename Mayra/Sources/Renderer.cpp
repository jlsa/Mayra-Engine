#include "Renderer.hpp"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    bool success = true;
    while (GLenum error = glGetError())
    {
        std::cout << "ERROR::OPENGL::" << error << "::" << file << "::" << line << "::" << function << std::endl;
        success = false;
    }
    return success;
}
