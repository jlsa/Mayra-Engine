//
//  gui.cpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//
#include <glad/glad.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>

#include "gui.hpp"



namespace Mayra
{
    void Gui::Initialize(GLFWwindow* window, std::string glsl_version)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
        _window = window;
    }

    void Gui::AddBoolParam(std::string name, bool param)
    {
        if (params.find(name) == params.end()) {
            params.insert(std::make_pair(name, param));
        }
    }

    void Gui::ToggleBoolParam(std::string name)
    {
        if (params.find(name) != params.end()) {
            params[name] = !params[name];
        }
    }

    bool Gui::GetBoolParam(std::string name)
    {
        if (params.find(name) != params.end())
        {
            return params.find(name)->second;
        }

        return false;
    }

    void Gui::PrepareRender()
    {
        NewFrame();
        TopBar();
        ImGui::Render();
    }

    void Gui::Render()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Gui::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Gui::TopBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Quit", "Cmd+Q")) {
                    glfwSetWindowShouldClose(_window, true);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Editor"))
            {
                if (ImGui::MenuItem("Show Lines")) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                if (ImGui::MenuItem("Show Fill")) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                std::map<std::string, bool>::iterator it = params.begin();
                while (it != params.end())
                {
                    if (ImGui::MenuItem(it->first.c_str())) {
                        ToggleBoolParam(it->first);
                    }
                    it++;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}
