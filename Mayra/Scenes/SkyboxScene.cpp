#include "EmptyScene.h"
#include "Input.hpp"
#include "Key.hpp"

namespace Mayra
{
    EmptyScene::EmptyScene()
      : m_ClearColor(glm::vec4(62.0f / 255.0f, 61.0f / 255.0f, 64.0f / 255.0f, 1.0f))
    {
        m_Camera = new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
        m_Projection = m_Camera->GetProjectionMatrix();
        m_Camera->SetPosition(glm::vec3(-0.33f, -0.66f, 0.0f));
        editorShader = new Mayra::Shader(SHADERS "EditorBg.vert", SHADERS "EditorBg.frag");
        editorShader->Bind();
        editorShader->SetVec3("cam_pos", m_Camera->GetPosition());
        editorShader->SetVec2("screen_size", glm::vec2(1280, 720));
        editorShader->SetMat4("view", m_Camera->GetViewMatrix());
        editorShader->Unbind();
    }

    void EmptyScene::OnUpdate(float /* deltaTime */)
    {}

    EmptyScene::~EmptyScene()
    {
        // delete all from vector
    }

    void EmptyScene::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        editorShader->Bind();
    }

    void EmptyScene::OnImGuiRender()
    {}
}
