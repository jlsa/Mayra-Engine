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
void handle_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

namespace Mayra
{
    Application::Application(WindowProps* props)
        : _camera(-1.6f, 1.6f, -0.9f, 0.9f), _props(props)
    {
        std::cout << "Application " << props->Title << " constructed" << std::endl;
    }

    Application::~Application()
    {
        Terminate();
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

        _window = Window::Instance();
        glfwSwapInterval(1);

        // Check for Valid Context
        if (Window::GetWindow() == nullptr) {
            fprintf(stderr, "Failed to Create OpenGL Context");
            return EXIT_FAILURE;
        }

        // Create Context and Load OpenGL Functions
        glfwMakeContextCurrent(Window::GetWindow());
        glfwSetFramebufferSizeCallback(Window::GetWindow(), framebuffer_size_callback);
        glfwSetScrollCallback(Window::GetWindow(), handle_scroll_callback);

        gladLoadGL();

        fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

        _gui = new Mayra::Gui();
        _gui->Initialize(Window::GetWindow(), glsl_version);

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
        sceneMenu->RegisterScene<SkyboxScene>("Skybox Scene");
        sceneMenu->RegisterScene<ModelLoadingScene>("Model Loading Scene");

        static double limitFPS = 1.0 / 60.0;

        double lastTime = glfwGetTime(), timer = lastTime;
        double deltaTime = 0, nowTime = 0;
        int frames = 0, updates = 0;

        while (glfwWindowShouldClose(Window::GetWindow()) == false)
        {
            Input::Instance()->OnUpdate();
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void handle_scroll_callback(GLFWwindow*, double xoffset, double yoffset)
{
    Mayra::Input::Instance()->HandleScroll(xoffset, yoffset);
}
