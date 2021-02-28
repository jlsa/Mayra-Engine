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
#include "Input.hpp"

#include "Scene.hpp"
#include <Scenes.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

namespace Mayra
{
    Application::Application(Mayra::WindowProps* props)
        : _camera(-1.6f, 1.6f, -0.9f, 0.9f), _props(props)
    {
        std::cout << "Application " << props->Title << " constructed" << std::endl;
    }

    Application::~Application()
    {
        Terminate();
    }

    void Application::HandleInput(Mayra::Window* window)
    {
        for (unsigned int i = 32; i < GLFW_KEY_LAST; i++)
        {
            if (glfwGetKey(window->Get(), i) == GLFW_PRESS)
                Mayra::Input::Instance()->HandleKeyDown(i);

            if (glfwGetKey(window->Get(), i) == GLFW_RELEASE)
                Mayra::Input::Instance()->HandleKeyRelease(i);

            if (glfwGetKey(window->Get(), i) == GLFW_REPEAT)
                Mayra::Input::Instance()->HandleKeyRepeat(i);
        }
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
        Scene* currentScene = nullptr;
        SceneMenu* sceneMenu = new SceneMenu(currentScene);
        currentScene = sceneMenu;

        sceneMenu->RegisterScene<SceneMultiTexturedQuad>("Multi Textured Quad");
        sceneMenu->RegisterScene<SceneGameObjectsTest>("Game Objects Test");
        sceneMenu->RegisterScene<SandboxScene>("Sandbox Scene");
        sceneMenu->RegisterScene<EmptyScene>("Empty Scene");
        sceneMenu->RegisterScene<FreeTypeFontScene>("FreeType2 Scene");
        sceneMenu->RegisterScene<BetterFontRenderingScene>("Better Font Rendering Scene");

        static double limitFPS = 1.0 / 60.0;

        double lastTime = glfwGetTime(), timer = lastTime;
        double deltaTime = 0, nowTime = 0;
        int frames = 0, updates = 0;

        while (glfwWindowShouldClose(_window->Get()) == false)
        {
            HandleInput(_window);
            // measure time
            nowTime = glfwGetTime();
            deltaTime += (nowTime - lastTime) / limitFPS;
            lastTime = nowTime;

            // only update at 60frames / s
            while (deltaTime >= 1.0)
            {
                if (currentScene)
                    currentScene->OnUpdate(deltaTime);
                updates++;
                deltaTime--;
            }

            Renderer::Instance()->Clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

            _gui->PrepareRender();

            // render at maximum possible frames
            if (currentScene)
                currentScene->OnRender();

            frames++;

            if (currentScene)
            {
                ImGui::Begin("--");
                if (currentScene != sceneMenu && ImGui::Button("<- Back"))
                {
                    delete currentScene;
                    currentScene = sceneMenu;
                }
                ImGui::End();
                currentScene->OnImGuiRender();
            }

            _gui->Render();

            if (glfwGetTime() - timer > 1.0)
            {
                timer++;
//                std::cout << "FPS: " << frames << " Updates: " << updates << std::endl;
                updates = 0, frames = 0;
            }

            glfwSwapBuffers(_window->Get());
            glfwPollEvents();
        }
        if (currentScene == sceneMenu)
            delete sceneMenu;
        else
            delete currentScene;
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
