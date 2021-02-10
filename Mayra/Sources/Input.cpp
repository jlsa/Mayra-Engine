#include <Mayra.hpp>
#include "Input.hpp"

namespace Mayra
{
    Input* Input::m_Instance = nullptr;

    Input* Input::Instance()
    {
        if (m_Instance == nullptr)
            m_Instance = new Input();

        return m_Instance;
    }

    Input::Input()
    {
        for (unsigned int i = 0; i < GLFW_KEY_LAST; i++)
        {
            m_Keys[i] = false;
            m_KeysDown[i] = false;
            m_KeysUp[i] = false;
        }

        for (unsigned int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
        {
            m_Mouse[i] = false;
            m_MouseUp[i] = false;
            m_MouseDown[i] = false;
        }
    }

    void Input::UpdateInput(Mayra::Window* window)
    {
        m_Window = window;

        glfwPollEvents();

        for (unsigned int i = 32; i < GLFW_KEY_LAST; i++)
        {
            if (glfwGetKey(window->Get(), i) == GLFW_PRESS)
                HandleKeyDown(i);

            if (glfwGetKey(window->Get(), i) == GLFW_RELEASE)
                HandleKeyRelease(i);

            if (glfwGetKey(window->Get(), i) == GLFW_REPEAT)
                HandleKeyRepeat(i);
        }

        glfwGetWindowSize(window->Get(), &m_WindowWidth, &m_WindowHeight);

        glfwGetCursorPos(window->Get(), &m_MouseX, &m_MouseY);

        m_MouseX = ((float)window->Props()->Width / m_WindowWidth) * m_MouseX;
        m_MouseY = ((float)window->Props()->Height / m_WindowHeight) * m_MouseY;

        for (unsigned int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
        {
            if (glfwGetMouseButton(window->Get(), i) == GLFW_PRESS)
                HandleMousePress(i);

            if (glfwGetMouseButton(window->Get(), i) == GLFW_RELEASE)
                HandleMouseRelease(i);
        }
    }

    glm::vec2 Input::GetMouse()
    {
        return glm::vec2(m_MouseX / 1000, m_MouseY / 1000);
    }

    bool Input::IsKey(KeyCode key)
    {
        return m_Keys[key];
    }

    bool Input::IsKeyDown(KeyCode key)
    {
        return m_KeysDown[key];
    }

    bool Input::IsKeyUp(KeyCode key)
    {
        return m_KeysUp[key];
    }

    bool Input::IsMouse(unsigned int button)
    {
        return m_Mouse[button];
    }

    bool Input::IsMouseUp(unsigned int button)
    {
        return m_MouseUp[button];
    }

    bool Input::IsMouseDown(unsigned int button)
    {
        return m_MouseDown[button];
    }

    void Input::HandleKeyDown(unsigned int key)
    {
        if (m_Keys[key] == false)
        {
            m_Keys[key] = true;
            m_KeysDown[key] = true;
        }
        else
        {
            m_KeysDown[key] = false;
        }
    }

    void Input::HandleKeyRepeat(unsigned int /* key */)
    {
//        std::cout << "Handle Key Repeat for key: " << key << std::endl;
    }

    void Input::HandleKeyRelease(unsigned int key)
    {
        if (m_Keys[key] == true)
        {
            m_Keys[key] = false;
            m_KeysUp[key] = true;
        }
        else
        {
            m_KeysUp[key] = false;
        }
    }

    void Input::HandleMousePress(unsigned int button)
    {
        if (m_Mouse[button] == false) // first time pressed down
        {
            m_Mouse[button] = true;
            m_MouseDown[button] = true;
            m_MouseUp[button] = false;
        }
        else
        {
            // not the first register of down but still pressed.
            m_MouseDown[button] = false;
        }
    }

    void Input::HandleMouseRelease(unsigned int button)
    {
        if (m_Mouse[button] == true) // still pressed
        {
            m_Mouse[button] = false;
            m_MouseUp[button] = true;
            m_MouseDown[button] = false;
            std::cout << "mouse_release::button::" << button << std::endl;
        }
        else
        {
            m_MouseUp[button] = false;
        }
    }

}
