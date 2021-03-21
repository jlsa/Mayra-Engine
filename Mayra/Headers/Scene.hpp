#ifndef Scene_h
#define Scene_h

#include <Mayra.hpp>
#include "Renderer.hpp"
#include <vector>

namespace Mayra
{
    class Scene
    {
    public:
        Scene() {}
        virtual ~Scene() {}
        
        virtual void OnUpdate(float) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    };

    class SceneMenu : public Scene
    {
    public:
        SceneMenu(Scene*& currentTestPointer);

        void OnImGuiRender() override;
        void OnRender() override;

        template<typename T>
        void RegisterScene(const std::string& name)
        {
            std::cout << "Registering scene: " << name << std::endl;
            m_Scenes.push_back(std::make_pair(name, []() { return new T(); }));
        }

        std::string GetCurrentSceneName()
        {
            return m_CurrentSceneName;
        }

        void SetCurrentSceneName(std::string sceneName)
        {
            m_CurrentSceneName = sceneName;
        }

    private:
        Scene*& m_CurrentScene;
        std::string m_CurrentSceneName;
        std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;
    };
}

#endif /* Scene_h */
