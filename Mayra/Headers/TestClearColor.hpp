#ifndef TestClearColor_h
#define TestClearColor_h

#include <Mayra.hpp>
#include <Test.hpp>

namespace Test
{
    class TestClearColor : public Test
    {
    public:
        TestClearColor();
        ~TestClearColor();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        float m_ClearColor[4];
    };
}

#endif /* TestClearColor_h */
