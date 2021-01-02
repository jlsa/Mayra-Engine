#ifndef TestClearColor_h
#define TestClearColor_h

#include <Mayra.hpp>
#include <Test.hpp>
#include "TestsManager.hpp"

namespace Test
{
    class TestClearColor : public Test
    {
    public:

        void OnAttach() override;
        void OnDetach() override;

        void Suspend() override;
        void Resume() override;

        void OnUpdate(TestsManager* testsManager, float deltaTime) override;
        void OnRender(TestsManager* testsManager) override;
        void OnImGuiRender(TestsManager* testsManager) override;

        static TestClearColor* Instance() {
            return &m_Instance;
        }

    protected:
        TestClearColor() {}

    private:
        static TestClearColor m_Instance;
        float m_ClearColor[4];
    };
}

#endif /* TestClearColor_h */
