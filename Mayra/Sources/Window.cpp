//
//  Window.cpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#include "Window.hpp"

namespace Mayra
{
    Window::Window(GLFWwindow* window)
    {
        _window = window;
        m_Props = new WindowProps("MayraEngine::DefaultWindowTitle");
    }

    Window::Window(WindowProps* props)
    {
        m_Props = props;
        create();
    }

    Window::~Window()
    {
        glfwDestroyWindow(_window);
    }

    void Window::create()
    {
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primary);
        std::cout << "Primary Monitor: " << mode->width << "x" << mode->height << std::endl;

        _window = glfwCreateWindow(m_Props->Width,
                                   m_Props->Height,
                                   m_Props->Title.c_str(),
                                   nullptr,
                                   nullptr);
    }

    GLFWwindow* Window::Get()
    {
        return _window;
    }

    Mayra::WindowProps* Window::Props() const
    {
        return m_Props;
    }
}
