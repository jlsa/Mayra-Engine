#include <Texture2D.hpp>

namespace Mayra
{
    Texture2D::Texture2D()
        : m_Width(0), m_Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), WrapS(GL_CLAMP_TO_EDGE), WrapT(GL_CLAMP_TO_EDGE), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR), m_RendererID(0),
            m_LocalBuffer(nullptr)
    {
    }

    Texture2D::~Texture2D()
    {
        GLCall(glDeleteTextures(1, &m_RendererID));
    }

    void Texture2D::Generate(int width, int height, int bpp, unsigned char* data)
    {
        GLCall(glGenTextures(1, &this->m_RendererID));
        m_LocalBuffer = data;
        m_Width = width;
        m_Height = height;
        m_BPP = bpp;

        // create texture
        Bind();

        if (m_BPP == 4) {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            GLCall(glBlendEquation(GL_FUNC_ADD));
        }
        
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, ImageFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));

        // set the texture wrapping/filtering options (on the current bound texture object)
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMin));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMax));

        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        // unbind texture
        UnBind();
    }

    void Texture2D::UnBind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Bind(unsigned int slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, this->m_RendererID));
    }

    Mayra::Texture2D* Texture2D::LoadFromFile(const std::string& filepath)
    {
        Mayra::Texture2D* texture = new Texture2D();
        // load image
        int width, height, bpp;
        stbi_set_flip_vertically_on_load(true);
        texture->m_LocalBuffer = stbi_load(filepath.c_str(), &width, &height, &bpp, 0);
        
        if (bpp == 4)
        {
            texture->InternalFormat = GL_RGBA8;
            texture->ImageFormat = GL_RGBA;
        }

        if (texture->m_LocalBuffer)
        {
            texture->Generate(width, height, bpp, texture->m_LocalBuffer);
            std::cout << "Texture generated" << std::endl;
            stbi_image_free(texture->m_LocalBuffer);
        }
        else
        {
            std::cout << "ERROR::TEXTURE2D_FAILED_TO_LOAD" << std::endl;
        }

        return texture;
    }
}
