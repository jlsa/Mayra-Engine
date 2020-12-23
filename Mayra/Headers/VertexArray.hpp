#ifndef VertexArray_h
#define VertexArray_h

#include <Mayra.hpp>
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace Mayra
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        void Bind() const;
        void Unbind() const;
    private:
        unsigned int m_RendererID;
    };
}

#endif /* VertexArray_h */
