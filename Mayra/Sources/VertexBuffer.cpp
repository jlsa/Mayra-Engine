#include "VertexBuffer.hpp"

namespace Mayra
{
    void VertexBuffer::CreateBuffer(const void* data, unsigned int size)
    {
        GLCall(glGenBuffers(1, &m_RendererID));
        Bind();
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    void VertexBuffer::CreateDynamicBuffer(unsigned int size)
    {
        GLCall(glGenBuffers(1, &m_RendererID));
        Bind();
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    void VertexBuffer::UpdateDynamicBuffer(const void* data)
    {
        // do something
    }

    VertexBuffer::~VertexBuffer()
    {
        GLCall(glDeleteBuffers(1, &m_RendererID));
    }

    void VertexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }
    void VertexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}
