#ifndef SkyboxScene_h
#define SkyboxScene_h

#include <Mayra.hpp>
#include <Scene.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"
#include "TextureCubemap.h"
#include "Camera.hpp"

namespace Mayra
{
    class SkyboxScene : public Scene
    {
    public:
        SkyboxScene();
        ~SkyboxScene();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        unsigned int cubemapTexture;
        unsigned int cubeVAO;
        unsigned int cubeVBO;
        unsigned int skyboxVAO;
        unsigned int skyboxVBO;
    private:
        glm::vec4 m_ClearColor;
        glm::mat4 m_Projection;
        OrthographicCamera *m_OrthoCamera;
        Camera* m_Camera;

        Mayra::Shader* m_SkyboxShader;

        Mayra::Shader* m_CubeShader;
        Mayra::Texture2D* m_CubeTexture;

        glm::vec2 m_ScreenSize;
    };
}

#endif /* SkyboxScene_h */
