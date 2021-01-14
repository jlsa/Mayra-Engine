#include "Scene.hpp"

namespace Mayra
{
    SceneMenu::SceneMenu(Scene*& currentScenePointer)
        : m_CurrentScene(currentScenePointer)
    {
    }

    void SceneMenu::OnImGuiRender()
    {
        ImGui::Begin("Scene Menu");
        for (auto& scene : m_Scenes)
        {
            if (ImGui::Button(scene.first.c_str()))
                m_CurrentScene = scene.second();
        }
        ImGui::End();
    }
}
