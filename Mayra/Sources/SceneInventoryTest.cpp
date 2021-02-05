#include "SceneInventoryTest.hpp"
#include "Sprite.hpp"

#include <string>

namespace Mayra
{
    SceneInventoryTest::SceneInventoryTest()
        : m_ClearColor(glm::vec4(1.0f)), m_Projection(glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f))
    {
        Sprite* spriteA = new Sprite(TEXTURES "block.png");
        spriteA->SetScale(glm::vec2(0.5f));
        spriteA->SetPosition(glm::vec2(0.5f, 0.5f));
        spriteA->SetRotation(0.0f);
        m_GameObjects.push_back(spriteA);

        m_Inventory = new Mayra::Inventory();
    }

    void SceneInventoryTest::OnUpdate(float) {}

    SceneInventoryTest::~SceneInventoryTest()
    {
        // delete all from vector
        delete m_Inventory;
    }

    void SceneInventoryTest::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        for (unsigned int i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects.at(i)->Render(nullptr);
        }
    }

    void SceneInventoryTest::OnImGuiRender()
    {
        {
            ImGui::Begin("Bags and Inventories.");
            std::string result = (!show) ? "Show" : "Hide";

            if (ImGui::Button((result + std::string(" Demo Window")).c_str()))
                show = !show;
            ImGui::End();
        }
        
        if (show)
            ImGui::ShowDemoWindow(&show);
    }
}
