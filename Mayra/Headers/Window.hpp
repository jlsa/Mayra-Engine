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

#include <glm/glm.hpp>

namespace Mayra
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        float Aspect;
        float AspectInverse;

        glm::vec2 GetCenter() { return glm::vec2(Width / 2.0f, Height / 2.0f); }
        
        WindowProps(const std::string& title = "Mayra Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height), Aspect(1.0f), AspectInverse(-1.0f)
        {}
    };

    class Window
    {
    public:
        ~Window();
        GLFWwindow* Get();
        static GLFWwindow* GetWindow();
        WindowProps* GetProps() const;

        Window(Window&) = delete; // copy prohibited
        void operator=(const Window&) = delete; // assignment prohibited
        Window& operator=(Window&&) = delete; // move assignment
        static Window* Instance();

    private:
        Window(WindowProps* props);
        static Window* m_Instance;
        
        GLFWwindow* m_Window;
        WindowProps* m_Props;
    };
}

#endif /* Window_h */
