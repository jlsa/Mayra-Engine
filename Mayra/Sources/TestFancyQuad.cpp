#include "TestFancyQuad.hpp"
#include "TestColoredQuad.hpp"
#include "TestClearColor.hpp"
#include "TestTexturedQuad.hpp"

namespace Test
{
    void TestFancyQuad::OnUpdate(float)
    {

    }

    TestFancyQuad::TestFancyQuad()
    {
        m_VertexBuffer = new Mayra::VertexBuffer();
        m_IndexBuffer = new Mayra::IndexBuffer();
        m_VertexArray = new Mayra::VertexArray();

        m_ClearColor = glm::vec4(0.941f, 1.0f, 1.0f, 1.0f);
        m_QuadColor = glm::vec4(0.255f, 0.412f, 0.882f, 1.0f);

        m_Shader = new Mayra::Shader(SHADERS "FancyQuad.vert", SHADERS "FancyQuad.frag");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };

        m_VertexBuffer->CreateBuffer(vertices, 4 * 3 * sizeof(float));

        Mayra::VertexBufferLayout layout;
        layout.Push<float>(3);
        m_VertexArray->AddBuffer(m_VertexBuffer, layout);

        m_IndexBuffer->Create(indices, sizeof(indices) / sizeof(indices[0]));
    }

    TestFancyQuad::~TestFancyQuad()
    {
        delete m_IndexBuffer;
        delete m_VertexBuffer;
        delete m_VertexArray;
        delete m_Shader;
    }

    void TestFancyQuad::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_Shader->Bind();
        m_Shader->SetVec4("u_Color", m_QuadColor);

        Mayra::Renderer::Instance()->Draw(m_VertexArray, m_IndexBuffer, m_Shader);
    }

    void TestFancyQuad::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
        ImGui::Separator();
        ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));
    }
}
