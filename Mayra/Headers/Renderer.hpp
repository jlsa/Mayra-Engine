#ifndef Renderer_hpp
#define Renderer_hpp

#include <Mayra.hpp>
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

namespace Mayra
{
    class Renderer
    {
    public:
        Renderer(Renderer&) = delete; // copy prohibited
        void operator=(const Renderer&) = delete; // assignment prohibited
        Renderer& operator=(Renderer&&) = delete; // move assignment
        static Renderer* Instance();

        void Clear() const;
        void Clear(const glm::vec4& clear_color) const;
        void Draw(const Mayra::VertexArray& va, const Mayra::IndexBuffer& ib, const Mayra::Shader& shader) const;

    private:
        static Renderer* m_Instance;
        Renderer();
    };
}

#endif /* Renderer_h */
