#ifndef Test_h
#define Test_h

#include "Renderer.hpp"

namespace Test
{
    class Test
    {
    public:
        Test() {}
        virtual ~Test() {}
        
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    };
}

#endif /* Test_h */
