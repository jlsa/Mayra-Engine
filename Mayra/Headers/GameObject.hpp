#ifndef GameObject_h
#define GameObject_h

#include <Mayra.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"

namespace Mayra
{
    class GameObject
    {
    public:
        GameObject() {}
        virtual ~GameObject() {}

        virtual void Update(float) {}
        virtual void Render() {}

        inline void AddChild(GameObject* child)
        {
            m_Children.push_back(child);
        }

        inline void DeleteChild(GameObject* child)
        {
            m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
        }

        const glm::vec2& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec2& position) { m_Position = position; }

        const glm::vec2& GetScale() const { return m_Scale; }
        void SetScale(const glm::vec2& scale) { m_Scale = scale; }

        const float& GetRotation() const { return m_Rotation; }
        void SetRotation(const float& rotation) { m_Rotation = rotation; }

        glm::vec2 m_Position;
        glm::vec2 m_Scale;
        float m_Rotation;

    protected:
        std::vector<GameObject*> m_Children;
    };
}


#endif /* GameObject_h */
