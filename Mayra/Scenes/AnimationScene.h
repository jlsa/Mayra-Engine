#ifndef AnimationScene_h
#define AnimationScene_h

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
#include <Skybox.h>

#include <Model.hpp>

namespace Mayra
{
    class AnimationScene : public Scene
    {
    public:
        AnimationScene();
        ~AnimationScene();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

        VertexArray* m_VertexArray;
        VertexBuffer* m_VertexBuffer;

        unsigned int cubeVAO;
        unsigned int cubeVBO;
    private:
        glm::vec4 m_ClearColor;
        glm::mat4 m_Projection;
        OrthographicCamera *m_OrthoCamera;
        Camera* m_Camera;

        Mayra::Shader* m_CubeShader;
        Mayra::Texture2D* m_CubeTexture;

        Mayra::Skybox* m_Skybox;

        Mayra::Shader* m_ModelShader;
        Mayra::Model* m_Model;

        glm::vec2 m_ScreenSize;

        bool m_Playmode;

        void OnPlay();
        void OnStop();
        void OnPause();
    };
}

#endif /* AnimationScene_h */
