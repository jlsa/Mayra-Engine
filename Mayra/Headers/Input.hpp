#ifndef Input_h
#define Input_h

#include <Mayra.hpp>

#include "Window.hpp"

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

        void UpdateInput(Mayra::Window* window);

        bool IsKey(KeyCode key);
        bool IsKeyDown(KeyCode key);
        bool IsKeyUp(KeyCode key);

        bool IsMouse(unsigned int button);
        bool IsMouseUp(unsigned int button);
        bool IsMouseDown(unsigned int button);

        void HandleKeyDown(unsigned int key);
        void HandleKeyRelease(unsigned int key);
        void HandleKeyRepeat(unsigned int key);

        void HandleMousePress(unsigned int button);
        void HandleMouseRelease(unsigned int button);

        void SetMouse(double x, double y);

    private:
        static Input* m_Instance;
        Input();

        bool m_Keys[GLFW_KEY_LAST];
        bool m_KeysUp[GLFW_KEY_LAST];
        bool m_KeysDown[GLFW_KEY_LAST];

        bool m_Mouse[GLFW_MOUSE_BUTTON_LAST];
        bool m_MouseUp[GLFW_MOUSE_BUTTON_LAST];
        bool m_MouseDown[GLFW_MOUSE_BUTTON_LAST];

        double m_MouseX;
        double m_MouseY;

        Mayra::Window* m_Window;
        int m_WindowWidth;
        int m_WindowHeight;
    };
}

#endif /* Input_h */
