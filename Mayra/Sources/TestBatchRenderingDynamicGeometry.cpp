#include "TestBatchRenderingDynamicGeometry.hpp"
#include <array>

namespace Test
{
    struct Vec2
    {
        float x, y;
    };

    struct Vec3
    {
        float x, y, z;
    };

    struct Vec4
    {
        float x, y, z, w;
    };

    struct Vertex
    {
        Vec3 Position;
        Vec4 Color;
        Vec2 TexCoords;
        float TexID;
    };

    static std::array<Vertex, 4> CreateQuad(float x, float y, float textureID)
    {
        float size = 1.0f;

        Vertex v0;
        v0.Position     = { x, y, 0.0f };
        v0.Color        = { 0.18f, 0.6f, 0.96f, 1.0f };
        v0.TexCoords    = { 0.0f, 0.0f };
        v0.TexID        = textureID;

        Vertex v1;
        v1.Position     = { x + size, y, 0.0f };
        v1.Color        = { 0.18f, 0.6f, 0.96f, 1.0f };
        v1.TexCoords    = { 1.0f, 0.0f };
        v1.TexID        = textureID;

        Vertex v2;
        v2.Position     = { x + size, y + size, 0.0f };
        v2.Color        = { 0.18f, 0.6f, 0.96f, 1.0f };
        v2.TexCoords    = { 1.0f, 1.0f };
        v2.TexID        = textureID;

        Vertex v3;
        v3.Position     = { x,  y + size, 0.0f };
        v3.Color        = { 0.18f, 0.6f, 0.96f, 1.0f };
        v3.TexCoords    = { 0.0f, 1.0f };
        v3.TexID        = textureID;

        return { v0, v1, v2, v3 };
    }
    TestBatchRenderingDynamicGeometry::TestBatchRenderingDynamicGeometry()
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
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        m_VertexBuffer->CreateDynamicBuffer(sizeof(Vertex));

        Mayra::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(4);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VertexArray->AddBuffer(m_VertexBuffer, layout);

        m_IndexBuffer->Create(indices, sizeof(indices) / sizeof(indices[0]));
    }

    void TestBatchRenderingDynamicGeometry::OnUpdate(float)
    {
        auto quad0 = CreateQuad(m_Quad0Position[0], m_Quad0Position[1], 0.0f);
        auto quad1 = CreateQuad(m_Quad1Position[0], m_Quad1Position[1], 1.0f);

        Vertex vertices[8];
        memcpy(vertices, quad0.data(), quad0.size() * sizeof(Vertex));
        memcpy(vertices + quad0.size(), quad1.data(), quad1.size() * sizeof(Vertex));

        // set dynamic buffer
        m_VertexArray->Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
    }

    TestBatchRenderingDynamicGeometry::~TestBatchRenderingDynamicGeometry()
    {
        delete m_IndexBuffer;
        delete m_VertexBuffer;
        delete m_VertexArray;
        delete m_Shader;
    }

    void TestBatchRenderingDynamicGeometry::OnRender()
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

    void TestBatchRenderingDynamicGeometry::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
        ImGui::Separator();

        ImGui::DragFloat2("Quad 0 Position", m_Quad0Position, 0.1f);
        ImGui::DragFloat2("Quad 1 Position", m_Quad1Position, 0.1f);

    }
}
