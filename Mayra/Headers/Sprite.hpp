#ifndef Sprite_h
#define Sprite_h

#include <Mayra.hpp>
#include "GameObject.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"

namespace Mayra
{
    class Sprite : public GameObject
    {
    public:
        Sprite(const std::string& filepath);
        
        ~Sprite();

        void Update(float deltaTime);
        void Render();
        
    private:
        void Init(float x, float y);

        Mayra::VertexArray* m_VertexArray;
        Mayra::VertexBuffer* m_VertexBuffer;
        Mayra::IndexBuffer* m_IndexBuffer;
        Mayra::Shader* m_Shader;
        Mayra::Texture2D* m_Texture;
        
        glm::mat4 m_Projection;
        std::string m_Filepath;
    };
}

#endif /* Sprite_h */
