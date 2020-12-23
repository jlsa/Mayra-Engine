#ifndef VertexBuffer_h
#define VertexBuffer_h
#include <Mayra.hpp>

namespace Mayra
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
    private:
        unsigned int m_RendererID;
    };
}

#endif /* VertexBuffer_h */
