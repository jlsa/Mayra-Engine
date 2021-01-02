#ifndef TestsManager_h
#define TestsManager_h

#include <Mayra.hpp>

namespace Test
{
    class Test;

    class TestsManager
    {
    public:
        TestsManager();
        ~TestsManager();

        void Attach();
        void Detach();

        void ChangeTest(Test* test);
        void PushTest(Test* test);
        void PopTest();

        void HandleEvents();
        void Update(float deltaTime);
        void Render();
        void RenderGui();

    private:
        std::vector<Test*> tests;
    };
}

#endif /* TestsManager_h */
