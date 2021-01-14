#include "SandboxScene.hpp"
#include "Sprite.hpp"
#include "Input.hpp"

namespace Mayra
{
    SandboxScene::SandboxScene()
        : m_ClearColor(glm::vec4(1.0f))
    {
        m_Camera = new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
        m_Projection = m_Camera->GetProjectionMatrix();
        m_Camera->SetPosition(glm::vec3(-0.33f, -0.66f, 0.0f));

        for (int y = 0; y < 20; ++y)
        {
            for (int x = 0; x < 10; ++x)
            {
                m_Board[y][x] = new Sprite(TEXTURES "block.png");
                m_Board[y][x]->SetPosition(glm::vec2(x * 0.075f, y * 0.075f));
                m_Board[y][x]->SetScale(glm::vec2(0.075f));
            }
        }
    }

    void SandboxScene::OnUpdate(float deltaTime)
    {
        float speed = 0.005f * deltaTime;
        glm::vec3 position = m_Camera->GetPosition();
        if (Input::Instance()->IsKeyDown(GLFW_KEY_UP))
        {
            position.y -= speed;
            std::cout << "UP" << std::endl;
        }
        if (Input::Instance()->IsKeyDown(GLFW_KEY_DOWN))
        {
            position.y += speed;
            std::cout << "DOWN" << std::endl;
        }
        if (Input::Instance()->IsKeyDown(GLFW_KEY_LEFT))
        {
            position.x += speed;
            std::cout << "LEFT" << std::endl;
        }
        if (Input::Instance()->IsKeyDown(GLFW_KEY_RIGHT))
        {
            position.x -= speed;
            std::cout << "RIGHT" << std::endl;
        }

        if (Input::Instance()->IsKeyDown(GLFW_KEY_SPACE))
        {
            std::cout << "(" << position.x << ", ";
            std::cout << position.y << ")" << std::endl;
        }
//        position.x += 0.0001f * deltaTime;
//        position.y += 0.0001f * deltaTime;
        m_Camera->SetPosition(position);
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
            m_GameObjects.at(i)->Render(m_Camera);
        }

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 10; x++)
            {
                m_Board[y][x]->Render(m_Camera);
            }
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
