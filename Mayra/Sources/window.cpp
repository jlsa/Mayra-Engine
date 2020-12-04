//
//  Window.cpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#include "window.hpp"

namespace Mayra
{
    Window::Window(GLFWwindow* window)
    {
        _window = window;
    }

    Window::Window(WindowProps* props)
    {
        _window = glfwCreateWindow(props->Width,
                                   props->Height,
                                   props->Title.c_str(),
                                   nullptr,
                                   nullptr);
    }

    Window::~Window()
    {
        glfwDestroyWindow(_window);
    }

    GLFWwindow* Window::Get()
    {
        return _window;
    }
}
