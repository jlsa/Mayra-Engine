#include "TextureCubemap.h"

namespace Mayra
{
    TextureCubemap::TextureCubemap()
        : m_Width(0), m_Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB),
            WrapS(GL_CLAMP_TO_EDGE), WrapT(GL_CLAMP_TO_EDGE), WrapR(GL_CLAMP_TO_EDGE),
            FilterMin(GL_LINEAR), FilterMax(GL_LINEAR), m_RendererID(0)
    {

    }

    TextureCubemap::~TextureCubemap()
    {
//        GLCall(glDeleteTextures(1, &m_RendererID));
    }

    void TextureCubemap::Unbind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void TextureCubemap::Bind(unsigned int slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_RendererID));
    }

//    void TextureCubemap::Generate(int width, int height, int bpp, unsigned char *data, int index)
//    {
//        GLCall(glGenTextures(1, &this->m_RendererID));
//        m_LocalBuffers[index] = data;
//        m_Width = width;
//        m_Height = height;
//        m_BPP = bpp;
//
//        Bind();
//        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
//
//        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, FilterMin));
//        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, FilterMax));
//        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, WrapS));
//        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, WrapT));
//        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, WrapR));
//        Unbind();
//    }

//    Mayra::TextureCubemap* TextureCubemap::LoadFromFiles(std::vector<std::string> faces, bool flip)
//    {
//        Mayra::TextureCubemap* texture = new TextureCubemap();
//
//        int width, height, bpp;
//        stbi_set_flip_vertically_on_load(flip);
//        for (unsigned int i = 0; i < faces.size(); i++)
//        {
////            texture->m_LocalBuffers[i] =
//            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &bpp, 0);
//
//            if (data)//texture->m_LocalBuffers[i])
//            {
////                texture->Generate(width, height, bpp, texture->m_LocalBuffers[i], i);
////                stbi_image_free(texture->m_LocalBuffers[i]);
//
//                texture->Generate(width, height, bpp, data, i);
//                stbi_image_free(data);
//            }
//            else
//            {
//                std::cout << "ERROR::TextureCubemap::FAILED_TO_LOAD" << std::endl;
//            }
//        }
//
//        return texture;
//    }

    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    // -------------------------------------------------------
    unsigned int TextureCubemap::LoadCubemap(std::vector<std::string> faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    Mayra::TextureCubemap* TextureCubemap::LoadFromFiles(std::vector<std::string> faces)
    {
        Mayra::TextureCubemap* texture = new TextureCubemap();
//        unsigned int textureID;
//        glGenTextures(1, &textureID);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        texture->Bind();

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        texture->Unbind();
        return texture;
    }
}
