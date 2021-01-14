#include "SandboxScene.hpp"
#include "Sprite.hpp"

namespace Mayra
{
    SandboxScene::SandboxScene()
        : m_ClearColor(glm::vec4(1.0f)), m_Projection(glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f))
    {
        Sprite* spriteA = new Sprite(TEXTURES "awesomeface.png");
        Sprite* spriteB = new Sprite(TEXTURES "container.jpg");
        Sprite* spriteC = new Sprite(TEXTURES "triangle.png");
        Sprite* spriteD = new Sprite(TEXTURES "wall.jpg");

        spriteA->SetScale(glm::vec2(0.5f));
        spriteB->SetScale(glm::vec2(0.5f));
        spriteC->SetScale(glm::vec2(0.5f));
        spriteD->SetScale(glm::vec2(0.5f));

        spriteA->SetPosition(glm::vec2(0.5f, 0.5f));
        spriteB->SetPosition(glm::vec2(-0.5f, 0.5f));
        spriteC->SetPosition(glm::vec2(-0.5f, -0.5f));
        spriteD->SetPosition(glm::vec2(0.5f, -0.5f));

        spriteA->SetRotation(0.0f);
        spriteB->SetRotation(45.0f);
        spriteC->SetRotation(90.0f);
        spriteD->SetRotation(180.0f);

        m_GameObjects.push_back(spriteA);
        m_GameObjects.push_back(spriteB);
        m_GameObjects.push_back(spriteC);
        m_GameObjects.push_back(spriteD);
    }

    void SandboxScene::OnUpdate(float deltaTime)
    {
        glm::vec2 scale = m_GameObjects.at(2)->GetScale();
        scale.x += sin(0.01f * deltaTime);
        scale.y += sin(0.01f * deltaTime);
        m_GameObjects.at(2)->SetScale(scale);
    }

    SandboxScene::~SandboxScene()
    {
        // delete all from vector
    }

    void SandboxScene::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        for (unsigned int i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects.at(i)->Render();
        }
    }

    void SandboxScene::OnImGuiRender()
    {
        ImGui::Begin("GameObjects");
        if (ImGui::Button("Add Sprite"))
        {
            Sprite* sprite = new Sprite(TEXTURES "awesomeface.png");
            sprite->SetScale(glm::vec2(0.5f));
            sprite->SetPosition(glm::vec2(0.5f, 0.5f));
            sprite->SetRotation(0.0f);
            m_GameObjects.push_back(sprite);
        }

        for (unsigned int i = 0; i < m_GameObjects.size(); i++)
        {
            std::string result;

            result = "GameObject: " + std::to_string(i) + "##" + std::to_string(i);
            if (ImGui::CollapsingHeader(result.c_str()))
            {
                result = "GameObject: " + std::to_string(i);
                ImGui::Text("%s", result.c_str());
                ImGui::Separator();
                result = "Position##" + std::to_string(i);
                ImGui::InputFloat2(result.c_str(), &m_GameObjects.at(i)->m_Position.x);
                result = "Scale##" + std::to_string(i);
                ImGui::InputFloat2(result.c_str(), &m_GameObjects.at(i)->m_Scale.x);
                result = "Rotation##" + std::to_string(i);
                float rotation = m_GameObjects.at(i)->GetRotation();
                ImGui::InputFloat(result.c_str(), &rotation);
                m_GameObjects.at(i)->SetRotation(rotation);
            }
        }
        ImGui::End();
    }
}
