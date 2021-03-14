//
//  Window.cpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#include "Window.hpp"

namespace Mayra
{
    Window* Window::m_Instance = nullptr;

    Window* Window::Instance()
    {
        Mayra::WindowProps* props = new Mayra::WindowProps("Mayra::Engine");
        if (m_Instance == nullptr)
            m_Instance = new Window(props);

        return m_Instance;
    }

    Window::Window(WindowProps* props)
    {
        m_Props = props;
        m_Window = glfwCreateWindow(props->Width,
                                    props->Height,
                                    props->Title.c_str(),
                                    nullptr,
                                    nullptr);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
    }

    GLFWwindow* Window::GetWindow()
    {
        return Instance()->Get();
    }

    GLFWwindow* Window::Get()
    {
        return m_Window;
    }

    void Window::SetTitle(std::string title)
    {
        m_Props->Title = title;
        glfwSetWindowTitle(GetWindow(), title.c_str());
    }

    WindowProps* Window::GetProps() const
    {
        return m_Props;
    }
}
