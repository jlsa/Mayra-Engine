#include "Sprite.hpp"

namespace Mayra
{
    Sprite::Sprite(const std::string& filepath)
        : m_Projection(glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f)), m_Filepath(filepath)
    {
        SetName("Sprite");
        Init(0.0f, 0.0f);
    }

    void Sprite::Init(float x, float y)
    {
        m_Position = glm::vec2(x, y);
        m_Scale = glm::vec2(1.0f);
        m_Color = glm::vec4(1.0f);
        SetRotation(0.0f);

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

        m_VertexBuffer = new Mayra::VertexBuffer();
        m_IndexBuffer = new Mayra::IndexBuffer();
        m_VertexArray = new Mayra::VertexArray();

        m_Shader = new Mayra::Shader(SHADERS "TexturedQuad.vert", SHADERS "TexturedQuad.frag");
        m_Texture = Mayra::Texture2D::LoadFromFile(m_Filepath);

        m_VertexBuffer->CreateBuffer(vertices, 4 * 5 * sizeof(float));

        Mayra::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VertexArray->AddBuffer(m_VertexBuffer, layout);

        m_IndexBuffer->Create(indices, sizeof(indices) / sizeof(indices[0]));
    }


    Sprite::~Sprite()
    {
        delete m_Texture;
        delete m_IndexBuffer;
        delete m_VertexBuffer;
        delete m_VertexArray;
        delete m_Shader;
    }
    
    void Sprite::Render(Mayra::OrthographicCamera* camera)
    {
        if (!GetEnabled())
            return;
        glm::vec2 origin = glm::vec2(0.0f);
        m_Texture->Bind();
        glm::vec3 cameraPosition = glm::vec3(0.0f);
        if (camera != nullptr)
            cameraPosition = camera->GetPosition();
        glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPosition);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));

        model = glm::translate(model, glm::vec3(origin.x * m_Scale.x, origin.y * m_Scale.y, 0.0f));
        model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-origin.x * m_Scale.x, -origin.y * m_Scale.y, 0.0f));

        model = glm::scale(model, glm::vec3(m_Scale, 1.0f));

        glm::mat4 projection = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f);

        glm::mat4 mvp = projection * view * model;

        m_Shader->Bind();
        m_Shader->SetInt("u_Texture", 0);
        m_Shader->SetVec4("u_Color", m_Color);

        m_Shader->SetMat4("u_MVP", mvp);
        Mayra::Renderer::Instance()->Draw(m_VertexArray, m_IndexBuffer, m_Shader);
    }

    void Sprite::Update(float)
    {
        if (!GetEnabled())
            return;
    }

    void Sprite::SetColor(glm::vec4 color)
    {
        m_Color = color;
    }
}
