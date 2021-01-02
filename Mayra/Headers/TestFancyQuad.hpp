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
        void OnAttach() override;
        void OnDetach() override;

        void Suspend() override;
        void Resume() override;

        void OnUpdate(TestsManager* testsManager, float deltaTime) override;
        void OnRender(TestsManager* testsManager) override;
        void OnImGuiRender(TestsManager* testsManager) override;

        static TestFancyQuad* Instance()
        {
            return &m_Instance;
        }

    protected:
        TestFancyQuad() {}

    private:
        static TestFancyQuad m_Instance;
        glm::vec4 m_ClearColor;
        glm::vec4 m_QuadColor;

        Mayra::VertexArray* m_VertexArray;
        Mayra::VertexBuffer* m_VertexBuffer;
        Mayra::IndexBuffer* m_IndexBuffer;
        Mayra::Shader* m_Shader;
    };
}

#endif /* TestFancyQuad_h */
