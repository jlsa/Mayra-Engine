#ifndef SandboxScene_h
#define SandboxScene_h

#include <Mayra.hpp>
#include <Scene.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"
#include "GameObject.hpp"

namespace Mayra
{
    class SandboxScene : public Scene
    {
    public:
        SandboxScene();
        ~SandboxScene();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_ClearColor;

        glm::mat4 m_Projection;

        OrthographicCamera* m_Camera;

        std::vector<Mayra::GameObject*> m_GameObjects;
        Mayra::GameObject* m_Board[20][10];
    };
}

#endif /* SandboxScene_h */
