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
        void OnAttach() override;
        void OnDetach() override;

        void Suspend() override;
        void Resume() override;
        
        void OnUpdate(TestsManager* testsManager, float deltaTime) override;
        void OnRender(TestsManager* testsManager) override;
        void OnImGuiRender(TestsManager* testsManager) override;

        static TestColoredQuad* Instance()
        {
            return &m_Instance;
        }

    protected:
        TestColoredQuad() {}

    private:
        static TestColoredQuad m_Instance;
        glm::vec4 m_ClearColor;
        glm::vec4 m_QuadColor;
        unsigned int VBO = 0;
        unsigned int VAO = 0;
        unsigned int EBO = 0;
        int shaderProgram = 0;
    };
}

#endif /* TestColoredQuad_h */
