#include "TestBatchRenderingColor.hpp"

namespace Test
{
    void TestBatchRenderingColor::OnUpdate(float)
    {

    }

    TestBatchRenderingColor::TestBatchRenderingColor()
    {
        m_VertexBuffer = new Mayra::VertexBuffer();
        m_IndexBuffer = new Mayra::IndexBuffer();
        m_VertexArray = new Mayra::VertexArray();

        m_ClearColor = glm::vec4(0.941f, 1.0f, 1.0f, 1.0f);

        m_Shader = new Mayra::Shader(SHADERS "BatchRenderingColor.vert", SHADERS "BatchRenderingColor.frag");
        m_TextureA = Mayra::Texture2D::LoadFromFile(TEXTURES "awesomeface.png");
        m_TextureB = Mayra::Texture2D::LoadFromFile(TEXTURES "triangle.png");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -1.5f, -0.5f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,  0.0f, 0.0f,  0.0f,
            -0.5f, -0.5f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,  1.0f, 0.0f,  0.0f,
            -0.5f,  0.5f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,  1.0f, 1.0f,  0.0f,
            -1.5f,  0.5f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,  0.0f, 1.0f,  0.0f,

             0.5f, -0.5f, 0.0f,  1.0f, 0.93f, 0.24f, 1.0f,  0.0f, 0.0f,  1.0f,
             1.5f, -0.5f, 0.0f,  1.0f, 0.93f, 0.24f, 1.0f,  1.0f, 0.0f,  1.0f,
             1.5f,  0.5f, 0.0f,  1.0f, 0.93f, 0.24f, 1.0f,  1.0f, 1.0f,  1.0f,
             0.5f,  0.5f, 0.0f,  1.0f, 0.93f, 0.24f, 1.0f,  0.0f, 1.0f,  1.0f
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        m_VertexBuffer->CreateBuffer(vertices, 8 * (3 + 4 + 2 + 1) * sizeof(float));

        Mayra::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(4);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VertexArray->AddBuffer(m_VertexBuffer, layout);

        m_IndexBuffer->Create(indices, sizeof(indices) / sizeof(indices[0]));
    }

    TestBatchRenderingColor::~TestBatchRenderingColor()
    {
        delete m_IndexBuffer;
        delete m_VertexBuffer;
        delete m_VertexArray;
        delete m_Shader;
    }

    void TestBatchRenderingColor::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_Shader->Bind();
        m_TextureA->Bind(0);
        m_TextureB->Bind(1);

        auto location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
        int samplers[2] = {0, 1};
        GLCall(glUniform1iv(location, 2, samplers));

        Mayra::Renderer::Instance()->Draw(m_VertexArray, m_IndexBuffer, m_Shader);
    }

    void TestBatchRenderingColor::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
    }
}
