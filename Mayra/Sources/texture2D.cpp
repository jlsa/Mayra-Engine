//
//  texture.cpp
//  Mayra
//
//  Created by Joel Hoekstra on 07/12/2020.
//

#include "texture2D.hpp"

namespace Mayra
{
    Texture2D::Texture2D()
        : Width(0), Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), WrapS(GL_REPEAT), WrapT(GL_REPEAT), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
    {
        glGenTextures(1, &this->ID);
    }

    void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
    {
        this->Width = width;
        this->Height = height;

        // create texture
        glBindTexture(GL_TEXTURE_2D, this->ID);

        glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->ImageFormat, GL_UNSIGNED_BYTE, data);

        // set the texture wrapping/filtering options (on the current bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);

        glGenerateMipmap(GL_TEXTURE_2D);

        // unbind texture
        this->UnBind();
    }

    void Texture2D::UnBind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, this->ID);
    }
}
