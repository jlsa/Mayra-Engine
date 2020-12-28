#ifndef IndexBuffer_h
#define IndexBuffer_h
#include <Mayra.hpp>

namespace Mayra
{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const { return m_Count; }
    private:
        unsigned int m_RendererID;
        unsigned int m_Count;
    };
}

#endif /* IndexBuffer_h */
