//
//  gui.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#ifndef gui_hpp
#define gui_hpp

#include <iostream>
#include <map>
#include <string>
#include <iterator>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Mayra
{
    class Gui
    {
    public:
        void Initialize(GLFWwindow* window, std::string glsl_version);
        void NewFrame();
        void PrepareRender();
        void Render();
        void AddBoolParam(std::string name, bool param);
        bool GetBoolParam(std::string name);
        void ToggleBoolParam(std::string name);

        // components/elements
        void TopBar();
        
    private:
        GLFWwindow* _window;
        std::map<std::string, bool> params;
    };
}

#endif /* gui_hpp */
