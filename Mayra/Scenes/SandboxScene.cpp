#include "SandboxScene.h"
#include "Sprite.hpp"
#include "Input.hpp"
#include "Key.hpp"

namespace Mayra
{
    SandboxScene::SandboxScene()
        : m_ClearColor(glm::vec4(1.000f, 0.627f, 0.478f, 1.0f))
    {
        m_Board = new Board(BOARD_WIDTH, BOARD_HEIGHT);
        m_Board->SetName("Board");
        m_Camera = new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
        m_Projection = m_Camera->GetProjectionMatrix();
        m_Camera->SetPosition(glm::vec3(-0.33f, -0.66f, 0.0f));

        m_Tetromino = new Tetromino();
        m_Tetromino->SetName("Tetromino");
        m_Tetromino->SetScale(glm::vec2(0.075f, 0.075f));
        m_GameObjects.push_back(m_Tetromino);
        m_GameObjects.push_back(m_Board);
    }

    void SandboxScene::OnUpdate(float deltaTime)
    {
        float distance = 0.075f;

        if (Input::Instance()->IsKeyDown(KeyCode::Up))
        {
            m_Tetromino->m_Position.y += distance;
            std::cout << "UP" << std::endl;
        }
        if (Input::Instance()->IsKeyDown(KeyCode::Down))
        {
            m_Tetromino->m_Position.y -= distance;
            std::cout << "DOWN" << std::endl;
        }
        if (Input::Instance()->IsKeyDown(KeyCode::Left))
        {
            m_Tetromino->m_Position.x -= distance;
            std::cout << "LEFT" << std::endl;
        }
        if (Input::Instance()->IsKeyDown(KeyCode::Right))
        {
            m_Tetromino->m_Position.x += distance;
            std::cout << "RIGHT" << std::endl;
        }

        if (Input::Instance()->IsKeyDown(KeyCode::Space))
        {
            m_Tetromino->m_Rotation += 90.0f;
            if (m_Tetromino->m_Rotation >= 360.0f)
                m_Tetromino->m_Rotation = 0.0f;
            std::cout << "Rotate" << std::endl;
        }

        for (unsigned int i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects.at(i)->Update(deltaTime);
        }
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
    }

    void SandboxScene::OnImGuiRender()
    {
        ImGui::Begin("GameObjects");
        if (ImGui::Button("Add Sprite"))
        {
            Sprite* sprite = new Sprite(TEXTURES "container2.png");
            sprite->SetScale(glm::vec2(0.5f));
            sprite->SetPosition(glm::vec2(0.5f, 0.5f));
            sprite->SetRotation(0.0f);
            m_GameObjects.push_back(sprite);
        }

        for (unsigned int i = 0; i < m_GameObjects.size(); i++)
        {
            std::string label;

            label = m_GameObjects.at(i)->GetName() + "##" + std::to_string(i);//"GameObject: " + std::to_string(i) + "##" + std::to_string(i);
            if (ImGui::CollapsingHeader(label.c_str()))
            {
                label = "GameObject: " + std::to_string(i);
                ImGui::Text("%s", label.c_str());
                label = "Enabled##" + std::to_string(i);
                ImGui::Checkbox(label.c_str(), &m_GameObjects.at(i)->m_Enabled);
                ImGui::Separator();
                label = "Position##" + std::to_string(i);
                ImGui::InputFloat2(label.c_str(), &m_GameObjects.at(i)->m_Position.x);
                label = "Scale##" + std::to_string(i);
                ImGui::InputFloat2(label.c_str(), &m_GameObjects.at(i)->m_Scale.x);
                label = "Rotation##" + std::to_string(i);
                float rotation = m_GameObjects.at(i)->GetRotation();
                ImGui::InputFloat(label.c_str(), &rotation);
                m_GameObjects.at(i)->SetRotation(rotation);

                ImGui::Separator();
                for (unsigned int j = 0; j < m_GameObjects.at(i)->GetChildren().size(); j++)
                {
                    Mayra::GameObject* go = m_GameObjects.at(i)->GetChildren().at(j);
                    ImGui::Text("%s", go->GetName().c_str());
                    label = "Enabled##" + std::to_string(i) + "##" + std::to_string(j);
                    ImGui::Checkbox(label.c_str(), &m_GameObjects.at(i)->GetChildren().at(j)->m_Enabled);
                    ImGui::Separator();
                    label = "Position##" + std::to_string(i) + "##" + std::to_string(j);
                    ImGui::InputFloat2(label.c_str(), &m_GameObjects.at(i)->GetChildren().at(j)->m_Position.x);
                    label = "Scale##" + std::to_string(i) + "##" + std::to_string(j);
                    ImGui::InputFloat2(label.c_str(), &m_GameObjects.at(i)->GetChildren().at(j)->m_Scale.x);
                    label = "Rotation##" + std::to_string(i) + "##" + std::to_string(j);
                    float rotation = m_GameObjects.at(i)->GetChildren().at(j)->GetRotation();
                    ImGui::InputFloat(label.c_str(), &rotation);
                    m_GameObjects.at(i)->SetRotation(rotation);
                }
            }
        }
        ImGui::End();
    }
}
