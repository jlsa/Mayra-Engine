// Local Headers
#include <Mayra.hpp>

// System Headers
#include <glad/glad.h>
#include <GLUT/glut.h>
//GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <iostream>

#include <Application.hpp>
#include <Color.hpp>
#include <Shader.hpp>
#include <Texture2D.hpp>
#include <OrthographicCamera.hpp>

#include <glm/gtx/matrix_decompose.hpp>

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"

#include "TestsManager.hpp"
#include "TestClearColor.hpp"
#include "TestColoredQuad.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

namespace Mayra
{
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

    Application::Application(Mayra::WindowProps* props)
        : _camera(-1.6f, 1.6f, -0.9f, 0.9f), _props(props)
    {
        std::cout << "Application " << props->Title << " constructed" << std::endl;
    }

    Application::~Application()
    {
        Terminate();
    }

    void Application::HandleInput(Mayra::Window*)
    {}

    int Application::Initialize()
    {
        glfwInit();
        std::string glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        _window = new Mayra::Window(_props);
        glfwSwapInterval(1);

        // Check for Valid Context
        if (_window->Get() == nullptr) {
            fprintf(stderr, "Failed to Create OpenGL Context");
            return EXIT_FAILURE;
        }

        // Create Context and Load OpenGL Functions
        glfwMakeContextCurrent(_window->Get());
        glfwSetFramebufferSizeCallback(_window->Get(), framebuffer_size_callback);
        gladLoadGL();

        fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

        _gui = new Mayra::Gui();
        _gui->Initialize(_window->Get(), glsl_version);

        return EXIT_SUCCESS;
    }

    void Application::Run()
    {
        Test::TestsManager* testsManager = new Test::TestsManager();

        testsManager->ChangeTest(Test::TestColoredQuad::Instance());

        while (glfwWindowShouldClose(_window->Get()) == false) {
            HandleInput(_window);
            _gui->PrepareRender();

            testsManager->HandleEvents();
            testsManager->Update(0.0f);
            testsManager->Render();
            
            {
                ImGui::Begin("Test Frame");
                testsManager->RenderGui();
                ImGui::End();
            }

            _gui->Render();

            glfwSwapBuffers(_window->Get());
            glfwPollEvents();
        }
        delete testsManager;
//        test.OnDetach();

//        GLCall(glDeleteVertexArrays(1, &VAO));
//        GLCall(glDeleteBuffers(1, &VBO));
//        GLCall(glDeleteBuffers(1, &EBO));
//        GLCall(glDeleteProgram(shaderProgram));
    }

    void Application::Terminate()
    {
        delete _window;
        delete _gui;
        delete _props;
        
        glfwTerminate();
    }
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    GLCall(glViewport(0, 0, width, height));
}
