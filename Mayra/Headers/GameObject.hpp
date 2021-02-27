#ifndef GameObject_h
#define GameObject_h

#include <Mayra.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"
#include "OrthographicCamera.hpp"

namespace Mayra
{
    class GameObject
    {
    public:
        GameObject() {}
        virtual ~GameObject() {}

        virtual void Update(float) {}
        virtual void Render(Mayra::OrthographicCamera*) {}

        inline void AddChild(GameObject* child)
        {
            m_Children.push_back(child);
        }

        inline void DeleteChild(GameObject* child)
        {
            m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
        }

        const std::vector<GameObject*> GetChildren() const
        {
            return m_Children;
        }

        const std::string& GetName() const { return m_Name; }
        void SetName(std::string name) { m_Name = name; }

        const glm::vec2& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec2& position) { m_Position = position; }

        const glm::vec2& GetScale() const { return m_Scale; }
        void SetScale(const glm::vec2& scale) { m_Scale = scale; }

        const float& GetRotation() const { return m_Rotation; }
        void SetRotation(const float& rotation) { std::cout << "rotation: " << rotation << std::endl;
            m_Rotation = rotation; }

        const bool& GetEnabled() const { return m_Enabled; }
        inline void SetEnabled(const bool& enabled) { m_Enabled = enabled; }
        inline void Enable() { m_Enabled = true; }
        inline void Disable() { m_Enabled = false; }

        glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
        glm::vec2 m_Scale = glm::vec2(1.0f, 1.0f);
        float m_Rotation = 0.0f;
        bool m_Enabled = true;
        
    protected:
        std::vector<GameObject*> m_Children;
        std::string m_Name = "GameObject";
    };
}


#endif /* GameObject_h */
