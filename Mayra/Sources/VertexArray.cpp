#include "VertexArray.hpp"

namespace Mayra
{
//    VertexArray::VertexArray()
//    {
//
//    }
    VertexArray::~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &m_RendererID));
    }

    void VertexArray::AddBuffer(VertexBuffer* vb, const VertexBufferLayout& layout)
    {
        if (m_RendererID == 0)
            GLCall(glGenVertexArrays(1, &m_RendererID));

        Bind();
        vb->Bind();
        const auto& elements = layout.GetElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++)
        {
            const auto& element = elements[i];
            GLCall(glEnableVertexAttribArray(i));
            GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), BUFFER_OFFSET(offset)));

            offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
        }
    }

    void VertexArray::Bind() const
    {
        GLCall(glBindVertexArray(m_RendererID));
    }

    void VertexArray::Unbind() const
    {
        GLCall(glBindVertexArray(0));
    }
}
