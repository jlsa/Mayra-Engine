//
//  TestFancyQuad.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 02/01/2021.
//

#ifndef TestFancyQuad_h
#define TestFancyQuad_h

#include <Mayra.hpp>
#include <Test.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

namespace Test
{
    class TestFancyQuad : public Test
    {
    public:
        TestFancyQuad();
        ~TestFancyQuad();
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
    };
}

#endif /* TestFancyQuad_h */
