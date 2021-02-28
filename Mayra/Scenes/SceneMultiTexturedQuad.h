#ifndef SceneMultiTexturedQuad_h
#define SceneMultiTexturedQuad_h

#include <Mayra.hpp>
#include <Scene.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"

namespace Mayra
{
    class SceneMultiTexturedQuad : public Scene
    {
    public:
        SceneMultiTexturedQuad();
        ~SceneMultiTexturedQuad();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_ClearColor;
        glm::vec4 m_QuadColor;

        Mayra::VertexArray* m_VertexArray;
        Mayra::VertexBuffer* m_VertexBuffer;
        Mayra::IndexBuffer* m_IndexBuffer;
        Mayra::Shader* m_Shader;
        Mayra::Texture2D* m_TextureA;
        Mayra::Texture2D* m_TextureB;

        glm::mat4 m_Projection;

        glm::vec3 m_TranslationA = glm::vec3(-0.5f, -0.5f, 0.0f);
        glm::vec3 m_ScaleA = glm::vec3(0.5f);
        glm::vec3 m_TranslationB = glm::vec3(0.5f, 0.5f, 0.0f);
        glm::vec3 m_ScaleB = glm::vec3(0.5f);
    };
}

#endif /* SceneMultiTexturedQuad_h */
