//
//  FontCharacter.h
//  Mayra
//
//  Created by Joel Hoekstra on 28/02/2021.
//

#ifndef FontCharacter_h
#define FontCharacter_h

namespace Mayra
{
    struct FontCharacter
    {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };
}

#endif /* FontCharacter_h */
