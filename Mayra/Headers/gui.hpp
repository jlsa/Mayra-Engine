//
//  gui.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#ifndef gui_hpp
#define gui_hpp

#include <iostream>

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
        
        // components/elements
        void TopBar();
        
    private:
        GLFWwindow* _window;
    };
}

#endif /* gui_hpp */
