// Preprocessor Directives
#ifndef MAYRA
#define MAYRA
#pragma once

// System Headers
//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>
//#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// System Headers
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
//     #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "functional"
#include "vector"
#include "algorithm"

// Mayra Headers
#include <Gui.hpp>
#include <Window.hpp>
#include <Application.hpp>

// Constants
#define ASSETS      "Assets/"
#define TEXTURES    ASSETS "Textures/"
#define SHADERS     "Shaders/"

#ifdef _MSC_VER
    #define DEBUG_BREAK __debugbreak()
#else
    #include <signal.h>
    #define DEBUG_BREAK raise(SIGINT)
#endif

#define ASSERT(x) if (!(x)) DEBUG_BREAK

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Should be placed in a GL specific file -- TODO
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    bool success = true;
    while (GLenum error = glGetError())
    {
        std::cout << "ERROR::OPENGL::" << error << "::" << file << "::" << line << "::" << function << std::endl;
        success = false;
    }
    return success;
}

#endif //~ Mayra Header
