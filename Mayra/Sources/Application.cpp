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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

namespace Mayra
{
    Application::Application(Mayra::WindowProps* props)
        : _camera(-1.6f, 1.6f, -0.9f, 0.9f), _props(props)
    {
        std::cout << "Application " << props->Title << " constructed" << std::endl;
        cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraLeft = glm::vec3(1.0f, 0.0f, 0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    Application::~Application()
    {
        Terminate();
    }

    void Application::HandleInput(Mayra::Window *window)
    {
        if (glfwGetKey(window->Get(), GLFW_KEY_1) == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (glfwGetKey(window->Get(), GLFW_KEY_2) == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // nice for an input manager one day!
        static bool reload_key_pressed = false;
        bool down = glfwGetKey(_window->Get(), GLFW_KEY_R);
        if (down && !reload_key_pressed)
        {
            reload_key_pressed = true;
        }
        else if (!down && reload_key_pressed)
        {
            reload_key_pressed = false;
        }

        float moveSpeed = 0.05f;
        glm::vec3 movePosition(0.0f, 0.0f, 0.0f);
        if (glfwGetKey(_window->Get(), GLFW_KEY_W) == GLFW_PRESS) {
            // forward
            movePosition -= moveSpeed * cameraUp;
        }
        else if (glfwGetKey(_window->Get(), GLFW_KEY_S) == GLFW_PRESS) {
            // backwards
            movePosition += moveSpeed * cameraUp;
        }
        else if (glfwGetKey(_window->Get(), GLFW_KEY_A) == GLFW_PRESS) {
            // left
            movePosition += moveSpeed * cameraLeft;
        }
        else if (glfwGetKey(_window->Get(), GLFW_KEY_D) == GLFW_PRESS) {
            // right
            movePosition -= moveSpeed * cameraLeft;
        }
        cameraPosition += movePosition;
    }

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
        Mayra::Renderer renderer;
        glm::vec4 clear_color = glm::vec4(Mayra::Color::purple, 1.0f);

        Mayra::Shader shader(SHADERS "SimpleTransform.vert", SHADERS "SimpleTransform.frag");
        Mayra::Texture2D texture = Mayra::Texture2D::LoadFromFile(TEXTURES "triangle.png");//"awesomeface.png");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            // positions          // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va;
        VertexBuffer vb(vertices, 4 * 5 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, sizeof(indices) / sizeof(indices[0]));

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        float scale = 1.0f;
        glm::mat4 Projection = glm::ortho(-1.6f / scale, 1.6f / scale, -0.9f / scale, 0.9f / scale, -1.0f, 1.0f);
        glm::mat4 identityViewMatrix(1.0f);
        glm::vec3 translation(0.0f);

        while (glfwWindowShouldClose(_window->Get()) == false) {
            HandleInput(_window);
            _gui->PrepareRender();

            renderer.Clear(clear_color);

            texture.Bind();

            glm::mat4 View = glm::translate(identityViewMatrix, cameraPosition);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 MVP = Projection * View * model;

            shader.Bind();
            shader.SetMat4("u_MVP", MVP);
//            shader.SetMat4("u_ViewProjection", _camera.GetViewProjectionMatrix());
            shader.SetInt("u_Texture", 0);
            shader.SetVec3("u_Color", Mayra::Color::white);

            renderer.Draw(va, ib, shader);

            {
                ImGui::SliderFloat3("Translation", &translation.x, -1.0f, 1.0f);
            }

            _gui->Render();
            // Flip Buffers and Draw
            glfwSwapBuffers(_window->Get());
            glfwPollEvents();
        }

        // de-allocate all resources once they've outlived their purpose:
        // --------------------------------------------------------------
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
//    UNUSED(window);
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    GLCall(glViewport(0, 0, width, height));
}
