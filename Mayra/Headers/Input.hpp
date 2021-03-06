#ifndef Input_h
#define Input_h

#include "MouseButton.h"
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

        template <size_t n>
        bool IsKey(KeyCode (&keys)[n])
        {
            for (unsigned long i = 0; i < n; i++)
            {
                if (IsKey(keys[i]))
                    return true;
            }
            return false;
        };

        bool IsAnyKey(KeyCode key1, KeyCode key2);
        bool AreBothKeys(KeyCode key1, KeyCode key2);

        bool IsKey(KeyCode key);
        bool IsKeyDown(KeyCode key);
        bool IsKeyUp(KeyCode key);

        bool IsMouseButton(MouseButton button);
        bool IsMouseButtonDown(MouseButton button);
        bool IsMouseButtonUp(MouseButton button);

        void HandleKeyDown(unsigned int key);
        void HandleKeyRelease(unsigned int key);
        void HandleKeyRepeat(unsigned int key);

        void HandleMouseButtonDown(unsigned int button);
        void HandleMouseButtonUp(unsigned int button);
        void HandleMouseButtonRepeat(unsigned int button);

        void HandleMouse(double xpos, double ypos);
        void HandleScroll(double xoffset, double yoffset);

        void SetMousePosition(glm::vec2 position);
        glm::vec2 GetMouseDelta() { return m_DeltaPosition; }
        glm::vec2 GetScrollOffset() { return m_ScrollOffset; }

    private:
        static Input* m_Instance;
        Input();

        bool m_Keys[GLFW_KEY_LAST];
        bool m_KeysUp[GLFW_KEY_LAST];
        bool m_KeysDown[GLFW_KEY_LAST];

        bool m_Mouse[GLFW_MOUSE_BUTTON_LAST];
        bool m_MouseUp[GLFW_MOUSE_BUTTON_LAST];
        bool m_MouseDown[GLFW_MOUSE_BUTTON_LAST];

        bool firstMouse;
        float lastX;
        float lastY;
        glm::vec2 m_DeltaPosition;
        glm::vec2 m_ScrollOffset;
    };
}

#endif /* Input_h */
