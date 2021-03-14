#include <Skybox.h>

namespace Mayra
{
    Skybox::Skybox()
    {
        m_VertexArray = new VertexArray();
        m_VertexBuffer = new VertexBuffer();

        m_VertexBuffer->CreateBuffer(Shapes::skybox.vertices, Shapes::skybox.verticesCount * Shapes::skybox.stride * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        m_VertexArray->AddBuffer(m_VertexBuffer, layout);

        std::vector<std::string> faces
        {
            TEXTURES "skybox/right.jpg",
            TEXTURES "skybox/left.jpg",
            TEXTURES "skybox/top.jpg",
            TEXTURES "skybox/bottom.jpg",
            TEXTURES "skybox/front.jpg",
            TEXTURES "skybox/back.jpg"
        };
        m_Texture = TextureCubemap::LoadFromFiles(faces);
        m_Shader = new Shader(SHADERS "skybox.vert", SHADERS "skybox.frag");

        m_Shader->Bind();
        m_Shader->SetInt("skybox", 0);
    }

    Skybox::~Skybox()
    {
        delete m_VertexArray;
        delete m_VertexBuffer;
        delete m_Texture;
        delete m_Shader;
    }

    void Skybox::Render(Camera* camera)
    {
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();// glm::perspective(glm::radians(camera->Zoom), m_ScreenSize.x / m_ScreenSize.y, 0.1f, 100.0f);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        m_Shader->Bind();
        view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
        m_Shader->SetMat4("view", view);
        m_Shader->SetMat4("projection", projection);


        m_VertexArray->Bind();
        m_Texture->Bind();
        Renderer::Instance()->Draw(m_VertexArray, m_Shader, Shapes::skybox.verticesCount);
//        glDrawArrays(GL_TRIANGLES, 0, Shapes::skybox.verticesCount);
        m_VertexArray->Unbind();
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    }
}
