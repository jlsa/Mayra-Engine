//
//  application.cpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

// Local Headers
#include "mayra.hpp"

// System Headers
#include <glad/glad.h>
#include <GLUT/glut.h>
//GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <iostream>

#include "application.hpp"
#include "color.hpp"
#include "shader.hpp"
#include "texture2D.hpp"

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
        : _props(props)
    {
        std::cout << "Application " << props->Title << " constructed" << std::endl;
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
        glm::vec4 clear_color = glm::vec4(Mayra::Color::gold, 1.0f);

        Mayra::Shader shader("../../../Mayra/Resources/shader3.vs", "../../../Mayra/Resources/shader3.fs");
        Mayra::Texture2D crate = LoadTextureFromFile("../../../Mayra/Resources/Assets/Textures/container.jpg", false);
        Mayra::Texture2D smile = LoadTextureFromFile("../../../Mayra/Resources/Assets/Textures/awesomeface.png", true);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        _gui->AddBoolParam("Show Crate", true);
        _gui->AddBoolParam("Show Smile", true);
        _gui->AddBoolParam("Flip Smile", true);
        _gui->AddFloatParam("Mix Percentage", 0.5f);
        
        while (glfwWindowShouldClose(_window->Get()) == false) {
            HandleInput(_window);

            _gui->PrepareRender();

            // Background Fill Color
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            crate.Bind();
            glActiveTexture(GL_TEXTURE1);
            smile.Bind();

            shader.Use();
            shader.SetInt("texture1", 0);
            shader.SetInt("texture2", 1);

            shader.SetBool("showTexture1", _gui->GetBoolParam("Show Crate"));
            shader.SetBool("showTexture2", _gui->GetBoolParam("Show Smile"));
            shader.SetBool("flipSmile", _gui->GetBoolParam("Flip Smile"));
            shader.SetFloat("mixPercentage", _gui->GetFloatParam("Mix Percentage"));

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
