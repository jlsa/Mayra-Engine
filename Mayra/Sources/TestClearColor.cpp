#include <TestClearColor.hpp>
#include "TestColoredQuad.hpp"
#include "TestFancyQuad.hpp"

namespace Test
{
    TestClearColor TestClearColor::m_Instance;

    void TestClearColor::OnUpdate(TestsManager*, float)
    {

    }

    void TestClearColor::OnAttach()
    {
        m_ClearColor[0] = 0.941f;
        m_ClearColor[1] = 1.0f;
        m_ClearColor[2] = 1.0f;
        m_ClearColor[3] = 1.0f;
    }

    void TestClearColor::OnDetach()
    {

    }

    void TestClearColor::Suspend()
    {

    }

    void TestClearColor::Resume()
    {

    }

    void TestClearColor::OnRender(TestsManager*)
    {
        GLCall(glClearColor(m_ClearColor[0],
                            m_ClearColor[1],
                            m_ClearColor[2],
                            m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnImGuiRender(TestsManager* testsManager)
    {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
        ImGui::Separator();
        if (ImGui::Button("Colored Quad Test"))
        {
            testsManager->ChangeTest(TestColoredQuad::Instance());
        }
        ImGui::SameLine();
        if (ImGui::Button("Fancy Quad Test"))
        {
            testsManager->ChangeTest(TestFancyQuad::Instance());
        }
    }
}
