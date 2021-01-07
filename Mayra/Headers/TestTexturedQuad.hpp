#ifndef TestTexturedQuad_h
#define TestTexturedQuad_h

#include <Mayra.hpp>
#include <Test.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"

namespace Test
{
    class TestTexturedQuad : public Test
    {
    public:
        TestTexturedQuad();
        ~TestTexturedQuad();
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
        Mayra::Texture2D* m_Texture;
    };
}

#endif /* TestTexturedQuad_h */
