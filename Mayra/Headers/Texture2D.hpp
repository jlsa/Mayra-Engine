//
//  texture.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 07/12/2020.
//

#ifndef texture_h
#define texture_h

// System Headers
#include <glad/glad.h>
#include <GLUT/glut.h>

// Local Headers
#include <Mayra.hpp>

namespace Mayra
{
    class Texture2D
    {
    public:
        Texture2D();

        unsigned int ID;
        unsigned int Width;
        unsigned int Height;

        unsigned int InternalFormat; // format of texture object
        unsigned int ImageFormat; // format of loaded image

        // texture configuration
        unsigned int WrapS;
        unsigned int WrapT;
        unsigned int FilterMin; // filtering mode if texture pixels < screen pixels
        unsigned int FilterMax; // filtering mode if texture pixels > screen pixels

        void Generate(unsigned int width, unsigned int height, unsigned char* data);
        void Bind() const;
        void UnBind() const;
    };
}

#endif /* texture_h */
