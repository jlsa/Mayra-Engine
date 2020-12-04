//
//  Window.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#ifndef Window_h
#define Window_h

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Mayra
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        
        WindowProps(const std::string& title = "Mayra Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {}
    };

    class Window
    {
        GLFWwindow* _window;
        
    public:
        Window(GLFWwindow* window);
        ~Window();
        Window(WindowProps* props);
        GLFWwindow* Get();
    };
}

#endif /* Window_h */
