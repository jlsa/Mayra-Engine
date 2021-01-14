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
        // do nothing <3
    }

    bool Input::IsKeyDown(int key)
    {
        if (m_Keys.find(key) != m_Keys.end())
            return m_Keys[key];
        return false;
    }

}
