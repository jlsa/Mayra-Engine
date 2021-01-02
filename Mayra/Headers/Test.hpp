#ifndef Test_h
#define Test_h

#include "Renderer.hpp"
#include "TestsManager.hpp"

namespace Test
{
    class Test
    {
    public:
        Test() {}
        virtual ~Test() {}

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;

        virtual void Suspend() = 0;
        virtual void Resume() = 0;
        
        virtual void OnUpdate(TestsManager*, float) {}
        virtual void OnRender(TestsManager*) {}
        virtual void OnImGuiRender(TestsManager*) {}

        void ChangeTest(TestsManager* testsManager, Test* test)
        {
            testsManager->ChangeTest(test);
        }
    };
}

#endif /* Test_h */
