//
//  application.cpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

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

Mayra::Texture2D LoadTextureFromFile(const char* file, bool alpha)
{
    Mayra::Texture2D texture;
    if (alpha)
    {
        texture.InternalFormat = GL_RGBA;
        texture.ImageFormat = GL_RGBA;
    }

    // load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data)
    {
        texture.Generate(width, height, data);
    }
    else
    {
        std::cout << "ERROR::TEXTURE2D_FAILED_TO_LOAD" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

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
        glm::vec4 clear_color = glm::vec4(Mayra::Color::chocolate, 1.0f);
        
        ShaderProgramSource source = ParseShader(SHADERS "default.glsl");
        
        Mayra::Shader* shader = new Mayra::Shader(SHADERS "SimpleTransform.vert", SHADERS "SimpleTransform.frag");
        Mayra::Texture2D smile = LoadTextureFromFile(TEXTURES "awesomeface.png", true);
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
        
        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        float scale = 1.0f;
        glm::mat4 Projection = glm::ortho(-1.6f / scale, 1.6f / scale, -0.9f / scale, 0.9f / scale, -1.0f, 1.0f);
        glm::mat4 identityViewMatrix(1.0f);
        
        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(1.0f, 0.0f, 0.0f));
        
        glm::mat4 Model2 = glm::mat4(1.0f);
        Model2 = glm::translate(Model2, cameraPosition);//glm::vec3(0.1f, 0.5f, 0.0f));
        Model2 = glm::scale(Model2, glm::vec3(0.5f, 0.5f, 0.0f));
        
        while (glfwWindowShouldClose(_window->Get()) == false) {
            HandleInput(_window);
            
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

            _gui->PrepareRender();

            glEnable(GL_DEPTH_TEST);
            // Background Fill Color
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            smile.Bind();
            
            glm::mat4 View = glm::translate(identityViewMatrix, cameraPosition);
            
            glm::mat4 MVP = Projection * View * Model;
            
            shader->Use();
            shader->SetMat4("u_MVP", MVP);
//            shader.SetMat4("u_ViewProjection", _camera.GetViewProjectionMatrix());
            shader->SetInt("image", 0);
            shader->SetVec3("u_Color", Mayra::Color::white);
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
            
            MVP = Projection * Model2;
            shader->Use();
            shader->SetMat4("u_MVP", MVP);
//            shader.SetMat4("u_ViewProjection", _camera.GetViewProjectionMatrix());
            shader->SetInt("image", 0);
            shader->SetVec3("u_Color", Mayra::Color::purple);
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0); // no need to unbind it every time

            _gui->Render();
            // Flip Buffers and Draw
            glfwSwapBuffers(_window->Get());
            glfwPollEvents();
        }

        // de-allocate all resources once they've outlived their purpose:
        // --------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
