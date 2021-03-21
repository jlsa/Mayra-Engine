#include "Scene.hpp"

namespace Mayra
{
    SceneMenu::SceneMenu(Scene*& currentScenePointer)
        : m_CurrentScene(currentScenePointer), m_CurrentSceneName(" - Menu - ")
    {
    }

    void SceneMenu::OnImGuiRender()
    {
        ImGui::Begin("Scene Menu");
        for (auto& scene : m_Scenes)
        {
            if (ImGui::Button(scene.first.c_str()))
            {
                m_CurrentScene = scene.second();
                SetCurrentSceneName(scene.first);
            }
        }
        ImGui::End();
    }

    void SceneMenu::OnRender()
    {
        glm::vec4 clearColor = glm::vec4(62.0f / 255.0f, 61.0f / 255.0f, 64.0f / 255.0f, 1.0f);
        GLCall(glClearColor(clearColor.r,
                            clearColor.g,
                            clearColor.b,
                            clearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
}
