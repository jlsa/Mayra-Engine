#ifndef TestFonts_h
#define TestFonts_h

#include <Mayra.hpp>
#include <Test.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Test
{
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2 Size;        // Size of the glyph
        glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
        unsigned int Advance;   // horizontal offset to advance to next glyph
    };
    
    class TestFonts : public Test
    {
    public:
        TestFonts();
        ~TestFonts();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::map<GLchar, Character> Characters;
        
    };
}

#endif /* TestFonts_h */
