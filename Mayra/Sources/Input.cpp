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

        firstMouse = true;
        lastX = 0;
        lastY = 0;
    }

    bool Input::IsAnyKey(KeyCode key1, KeyCode key2)
    {
        return IsKey(key1) || IsKey(key2);
    }

    bool Input::AreBothKeys(KeyCode key1, KeyCode key2)
    {
        return IsKey(key1) && IsKey(key2);
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

    void Input::HandleMouse(double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        glm::vec2 offset;
        offset.x = xpos - lastX;
        offset.y = lastY - ypos; // reversed since y-coordinates go from bottom to top

        m_DeltaPosition = offset;
        lastX = xpos;
        lastY = ypos;
        //    camera.ProcessMouseMovement(xoffset, yoffset);
    }

    void Input::SetMousePosition(glm::vec2 position)
    {
        HandleMouse((double)position.x, (double)position.y);
    }

    void Input::HandleScroll(double xoffset, double yoffset)
    {
        m_ScrollOffset = glm::vec2((float)xoffset, (float)yoffset);
    }

}
