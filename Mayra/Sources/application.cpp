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

        Mayra::Shader shader("../../../Mayra/Resources/shader.vs", "../../../Mayra/Resources/shader.fs");
        Mayra::Shader shader2("../../../Mayra/Resources/shader2.vs", "../../../Mayra/Resources/shader2.fs");
        Mayra::Shader shader3("../../../Mayra/Resources/shader3.vs", "../../../Mayra/Resources/shader3.fs");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float offset = 0.0f;
        float vertices[] = {
             0.5f + offset,  0.5f + offset, 0.0f,     0.0f, 1.0f, 1.0f,  // top right
             0.5f + offset, -0.5f + offset, 0.0f,     0.0f, 0.0f, 1.0f,  // bottom right
            -0.5f + offset, -0.5f + offset, 0.0f,     0.0f, 0.0f, 0.0f,  // bottom left
            -0.5f + offset,  0.5f + offset, 0.0f,     0.0f, 1.0f, 0.0f   // top left
        };
        offset = 0.5f;
        float vertices2[] = {
             0.3f - offset,  0.3f - offset, 0.0f,  // top right
             0.3f - offset, -0.3f - offset, 0.0f,  // bottom right
            -0.3f - offset, -0.3f - offset, 0.0f,  // bottom left
            -0.3f - offset,  0.3f - offset, 0.0f   // top left
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        unsigned int indices2[] = {  // note that we start from 0!
            0, 1, 2,   // first triangle
            2, 3, 0    // second triangle
        };

        float triangle[] = {
            // positions         // colors
             0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, // bottom left
             0.0f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.5f, 1.0f  // top
        };

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // set the texture wrapping/filtering options (on the current bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load("../../../Mayra/Resources/Assets/Textures/container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "ERROR::TEXTURE_FAILED_TO_LOAD" << std::endl;
        }

        stbi_image_free(data);

        // Buffers
        unsigned int VBOs[3], VAOs[3], EBOs[2];
        glGenVertexArrays(3, VAOs);
        glGenBuffers(3, VBOs);
        glGenBuffers(2, EBOs);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAOs[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);

        // repeat, second triangle
        glBindVertexArray(VAOs[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // third triangle
        glBindVertexArray(VAOs[2]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        while (glfwWindowShouldClose(_window->Get()) == false) {
            HandleInput(_window);
            _gui->PrepareRender();

            // Background Fill Color
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Use();

            glBindVertexArray(VAOs[0]);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

            shader2.Use();
            shader2.SetFloat("xOffset", 0.75f);
            shader2.SetVec3("color", Mayra::Color::mediumpurple);

            glBindVertexArray(VAOs[1]);
            glDrawElements(GL_TRIANGLES, sizeof(indices2) / sizeof(indices2[0]), GL_UNSIGNED_INT, 0);

            shader3.Use();
            glBindVertexArray(VAOs[2]);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindVertexArray(0); // no need to unbind it every time

            _gui->Render();
            // Flip Buffers and Draw
            glfwSwapBuffers(_window->Get());
            glfwPollEvents();
        }
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
