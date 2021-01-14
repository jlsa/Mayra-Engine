#ifndef TestBatchRenderingColor_h
#define TestBatchRenderingColor_h

#include <Mayra.hpp>
#include <Test.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Renderer.hpp"

namespace Test
{
    class TestBatchRenderingColor : public Test
    {
    public:
        TestBatchRenderingColor();
        ~TestBatchRenderingColor();
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
    };
}

#endif /* TestBatchRenderingColor_h */
