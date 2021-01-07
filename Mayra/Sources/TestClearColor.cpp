#include <TestClearColor.hpp>
#include "TestColoredQuad.hpp"
#include "TestFancyQuad.hpp"
#include "TestTexturedQuad.hpp"

namespace Test
{
    TestClearColor::TestClearColor()
    {
        m_ClearColor[0] = 0.941f;
        m_ClearColor[1] = 1.0f;
        m_ClearColor[2] = 1.0f;
        m_ClearColor[3] = 1.0f;
    }
    void TestClearColor::OnUpdate(float)
    {

    }

    void TestClearColor::OnRender()
    {
        GLCall(glClearColor(m_ClearColor[0],
                            m_ClearColor[1],
                            m_ClearColor[2],
                            m_ClearColor[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
        ImGui::Separator();
    }
}
