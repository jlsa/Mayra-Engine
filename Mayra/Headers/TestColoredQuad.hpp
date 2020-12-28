#ifndef TestColoredQuad_h
#define TestColoredQuad_h

#include <Mayra.hpp>
#include <Test.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

namespace Test
{
    class TestColoredQuad : public Test
    {
    public:
        TestColoredQuad();
        ~TestColoredQuad();

        void SetRenderer(Mayra::Renderer) override;
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_ClearColor;
        glm::vec4 m_QuadColor;
        unsigned int VBO = 0;
        unsigned int VAO = 0;
        unsigned int EBO = 0;
        int shaderProgram = 0;
    };
}

#endif /* TestColoredQuad_h */
