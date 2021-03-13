#ifndef VectorScene_h
#define VectorScene_h

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
    class VectorScene : public Scene
    {
    public:
        VectorScene();
        ~VectorScene();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

        VertexArray* m_VertexArray;
        VertexBuffer* m_VertexBuffer;

        VertexArray* m_SkyboxVA;
        VertexBuffer* m_SkyboxVB;

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
        Mayra::TextureCubemap* m_SkyboxTexture;

        glm::vec2 m_ScreenSize;

        bool m_Playmode;

        void OnPlay();
        void OnStop();
        void OnPause();
    };
}

#endif /* VectorScene_h */
