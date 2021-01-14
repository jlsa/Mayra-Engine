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

        template<typename T>
        void RegisterScene(const std::string& name)
        {
            std::cout << "Registering scene: " << name << std::endl;
            m_Scenes.push_back(std::make_pair(name, []() { return new T(); }));
        }

    private:
        Scene*& m_CurrentScene;
        std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;
    };
}

#endif /* Scene_h */
