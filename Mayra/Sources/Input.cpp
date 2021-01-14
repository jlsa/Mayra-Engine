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

}
