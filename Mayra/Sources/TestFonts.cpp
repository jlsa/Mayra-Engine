#include "TestFonts.hpp"

namespace Test
{
    TestFonts::TestFonts()
    {
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

//        Mayra::Shader* shader = new Mayra::Shader(SHADERS "Font.vert", SHADERS "Font.frag");
//        glm::mat4 m_Projection = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f);
//        shader->Bind();
//        shader->SetMat4("u_Projection", m_Projection);

//        FT_Library ft;
//        if (FT_Init_FreeType(&ft))
//        {
//            std::cout << "ERROR::FREETYPE::LIBRARY_INIT_FAILED" << std::endl;
//            // best to throw something here - handle errors better
//        }

    }

    void TestFonts::OnUpdate(float)
    {

    }

    TestFonts::~TestFonts()
    {
    }

    void TestFonts::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestFonts::OnImGuiRender()
    {
    }
}
