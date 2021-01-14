#ifndef Input_h
#define Input_h

#include "Key.hpp"
#include <map>

namespace Mayra
{
    class Input
    {
    public:
        Input(Input&) = delete; // copy prohibited
        void operator=(const Input&) = delete; // assignment prohibited
        Input& operator=(Input&&) = delete; // move assignment
        static Input* Instance();

        bool IsKey(KeyCode key);
        bool IsKeyDown(KeyCode key);
        bool IsKeyUp(KeyCode key);

        void HandleKeyDown(unsigned int key);
        void HandleKeyRelease(unsigned int key);
        void HandleKeyRepeat(unsigned int key);
    private:
        static Input* m_Instance;
        Input();

        bool m_Keys[GLFW_KEY_LAST];
        bool m_KeysUp[GLFW_KEY_LAST];
        bool m_KeysDown[GLFW_KEY_LAST];
    };
}

#endif /* Input_h */
