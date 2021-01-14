#ifndef SceneGameObjectsTest_h
#define SceneGameObjectsTest_h

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
    class SceneGameObjectsTest : public Scene
    {
    public:
        SceneGameObjectsTest();
        ~SceneGameObjectsTest();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_ClearColor;

        glm::mat4 m_Projection;

        std::vector<Mayra::GameObject*> m_GameObjects;
    };
}

#endif /* SceneGameObjectsTest_h */
