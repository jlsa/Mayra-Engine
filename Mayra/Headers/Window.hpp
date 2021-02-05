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
        float Aspect;
        float AspectInverse;
        int VSync;
        
        WindowProps(const std::string& title = "Mayra Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height), Aspect(1.0f), AspectInverse(-1.0f), VSync(1)
        {}
    };

    class Window
    {
    public:
        Window(GLFWwindow* window);
        ~Window();
        Window(WindowProps* props);
        GLFWwindow* Get();
        Mayra::WindowProps* Props() const;

    private:
        GLFWwindow* _window;
        WindowProps* m_Props;

        void create();
    };
}

#endif /* Window_h */
