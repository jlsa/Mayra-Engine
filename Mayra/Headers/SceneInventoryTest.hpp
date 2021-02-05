#ifndef SceneInventoryTest_h
#define SceneInventoryTest_h

#include <Mayra.hpp>
#include <Scene.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"
#include "GameObject.hpp"

namespace Mayra
{
    struct Item
    {
        std::string Name;
    };

    struct Bag : public Item
    {
        std::string Name;
        int Slots;
    };

    class Inventory
    {
    public:
        std::vector<Mayra::Item> All() const { return m_Items; }
        inline void Add(Mayra::Item& item)
        {
            m_Items.push_back(item);
        }

        inline void Remove(Mayra::Item&)
        {
            // let's implement this later shall we?
        }
    private:
        std::vector<Mayra::Item> m_Items;
    };

    class SceneInventoryTest : public Scene
    {
    public:
        SceneInventoryTest();
        ~SceneInventoryTest();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_ClearColor;

        glm::mat4 m_Projection;

        std::vector<Mayra::GameObject*> m_GameObjects;
        Mayra::Inventory* m_Inventory;
        bool show = false;
    };
}

#endif /* SceneInventoryTest_h */
