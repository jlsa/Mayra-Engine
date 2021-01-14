#ifndef Input_h
#define Input_h

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

        bool IsKeyDown(int key);
        void KeyPress(int key)
        {
            m_Keys[key] = true;
        }

        void KeyRelease(int key)
        {
            m_Keys[key] = false;
        }
    private:
        static Input* m_Instance;
        Input();
        std::map<int, bool> m_Keys;
    };
}

#endif /* Input_h */
