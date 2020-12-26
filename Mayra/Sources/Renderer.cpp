#include "Renderer.hpp"

namespace Mayra
{
    Renderer::Renderer()
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    void Renderer::Clear() const
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }

    void Renderer::Clear(const glm::vec4& clear_color) const
    {
        Clear();
        // Background Fill Color
        GLCall(glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void Renderer::Draw(const Mayra::VertexArray& va, const Mayra::IndexBuffer& ib, const Mayra::Shader& shader) const
    {
        shader.Bind();
        va.Bind();
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
    }
}
