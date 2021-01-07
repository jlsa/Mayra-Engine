#include "TestTexturedQuad.hpp"
#include "TestColoredQuad.hpp"
#include "TestClearColor.hpp"
#include "TestFancyQuad.hpp"

namespace Test
{
    TestTexturedQuad::TestTexturedQuad()
    {
        float vertices[] = {
            // positions          // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VertexBuffer = new Mayra::VertexBuffer();
        m_IndexBuffer = new Mayra::IndexBuffer();
        m_VertexArray = new Mayra::VertexArray();

        m_ClearColor = glm::vec4(0.941f, 1.0f, 1.0f, 1.0f);
        m_QuadColor = glm::vec4(0.255f, 0.412f, 0.882f, 1.0f);

        m_Shader = new Mayra::Shader(SHADERS "TexturedQuad.vert", SHADERS "TexturedQuad.frag");
        m_Texture = Mayra::Texture2D::LoadFromFile(TEXTURES "awesomeface.png");

        m_VertexBuffer->CreateBuffer(vertices, 4 * 5 * sizeof(float));

        Mayra::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VertexArray->AddBuffer(m_VertexBuffer, layout);

        m_IndexBuffer->Create(indices, sizeof(indices) / sizeof(indices[0]));
    }

    void TestTexturedQuad::OnUpdate(float)
    {

    }

    TestTexturedQuad::~TestTexturedQuad()
    {
        delete m_Texture;
        delete m_IndexBuffer;
        delete m_VertexBuffer;
        delete m_VertexArray;
        delete m_Shader;
    }

    void TestTexturedQuad::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_Texture->Bind();

        m_Shader->Bind();
        m_Shader->SetInt("u_Texture", 0);
        m_Shader->SetVec4("u_Color", m_QuadColor);
        glm::mat4 identity = glm::mat4(1.0f);
        m_Shader->SetMat4("u_MVP", identity);

        Mayra::Renderer::Instance()->Draw(m_VertexArray, m_IndexBuffer, m_Shader);
    }

    void TestTexturedQuad::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
        ImGui::Separator();
        ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));
    }
}
