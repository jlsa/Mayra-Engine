#ifndef TestBatchRenderingDynamicGeometry_h
#define TestBatchRenderingDynamicGeometry_h

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
    class TestBatchRenderingDynamicGeometry : public Test
    {
    public:
        TestBatchRenderingDynamicGeometry();
        ~TestBatchRenderingDynamicGeometry();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_ClearColor;

        Mayra::VertexArray* m_VertexArray;
        Mayra::VertexBuffer* m_VertexBuffer;
        Mayra::IndexBuffer* m_IndexBuffer;
        Mayra::Shader* m_Shader;

        Mayra::Texture2D* m_TextureA;
        Mayra::Texture2D* m_TextureB;

        float m_Quad0Position[2] = { -1.5f, -0.5f };
        float m_Quad1Position[2] = { -0.5f, -0.5f };
    };
}

#endif /* TestBatchRenderingDynamicGeometry_h */
