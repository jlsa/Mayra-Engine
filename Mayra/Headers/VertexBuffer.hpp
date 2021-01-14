#ifndef VertexBuffer_h
#define VertexBuffer_h
#include <Mayra.hpp>

namespace Mayra
{
    class VertexBuffer
    {
    public:
        ~VertexBuffer();
        
        void CreateBuffer(const void* data, unsigned int size);
        void CreateDynamicBuffer(unsigned int size);
        void UpdateDynamicBuffer(const void* data);

        void Bind() const;
        void Unbind() const;
    private:
        unsigned int m_RendererID;
    };
}

#endif /* VertexBuffer_h */
