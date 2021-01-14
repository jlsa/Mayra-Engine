#include "SceneMultiTexturedQuad.hpp"

namespace Mayra
{
    SceneMultiTexturedQuad::SceneMultiTexturedQuad()
        : m_Projection(glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f))
    {
        float verticesA[] = {
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

        m_VertexBuffer = new Mayra::VertexBuffer();
        m_IndexBuffer = new Mayra::IndexBuffer();
        m_VertexArray = new Mayra::VertexArray();

        m_ClearColor = glm::vec4(0.941f, 1.0f, 1.0f, 1.0f);
        m_QuadColor = glm::vec4(0.255f, 0.412f, 0.882f, 1.0f);

        m_Shader = new Mayra::Shader(SHADERS "TexturedQuad.vert", SHADERS "TexturedQuad.frag");
        m_TextureA = Mayra::Texture2D::LoadFromFile(TEXTURES "awesomeface.png");
        m_TextureB = Mayra::Texture2D::LoadFromFile(TEXTURES "triangle.png");

        m_VertexBuffer->CreateBuffer(verticesA, 4 * 5 * sizeof(float));

        Mayra::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VertexArray->AddBuffer(m_VertexBuffer, layout);

        m_IndexBuffer->Create(indices, sizeof(indices) / sizeof(indices[0]));
    }

    void SceneMultiTexturedQuad::OnUpdate(float)
    {

    }

    SceneMultiTexturedQuad::~SceneMultiTexturedQuad()
    {
        delete m_TextureA;
        delete m_TextureB;
        delete m_IndexBuffer;
        delete m_VertexBuffer;
        delete m_VertexArray;
        delete m_Shader;
    }

    void SceneMultiTexturedQuad::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        {
            glm::mat4 identity = glm::mat4(1.0f);
            m_TextureB->Bind();
            glm::mat4 view = glm::translate(identity, glm::vec3(0.0f));
            glm::mat4 model = glm::translate(identity, m_TranslationB);
            model = glm::scale(model, m_ScaleB);
            glm::mat4 mvp = m_Projection * view * model;

            m_Shader->Bind();
            m_Shader->SetInt("u_Texture", 0);
            m_Shader->SetVec4("u_Color", m_QuadColor);
            m_Shader->SetMat4("u_MVP", mvp);
            Mayra::Renderer::Instance()->Draw(m_VertexArray, m_IndexBuffer, m_Shader);
        }
        
        {
            glm::mat4 identity = glm::mat4(1.0f);
            m_TextureA->Bind();
            glm::mat4 view = glm::translate(identity, glm::vec3(0.0f));
            glm::mat4 model = glm::translate(identity, m_TranslationA);
            model = glm::scale(model, m_ScaleA);
            glm::mat4 mvp = m_Projection * view * model;

            m_Shader->Bind();
            m_Shader->SetInt("u_Texture", 0);
            m_Shader->SetVec4("u_Color", m_QuadColor);

            m_Shader->SetMat4("u_MVP", mvp);
            Mayra::Renderer::Instance()->Draw(m_VertexArray, m_IndexBuffer, m_Shader);
        }


    }

    void SceneMultiTexturedQuad::OnImGuiRender()
    {
        ImGui::Begin("Multi Textured Quad");
        ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));

        ImGui::Separator();

        ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));

        ImGui::Separator();

        ImGui::InputFloat3("Translation", &m_TranslationA.x);
        ImGui::SliderFloat3("Scale", &m_ScaleA.x, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::InputFloat3("Translation##second", &m_TranslationB.x);
        ImGui::SliderFloat3("Scale##second", &m_ScaleB.x, 0.0f, 1.0f);
        ImGui::End();
    }
}
