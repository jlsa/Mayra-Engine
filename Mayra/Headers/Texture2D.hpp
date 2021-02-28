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
        ~Texture2D();

        int m_Width;
        int m_Height;
        int m_BPP;

        unsigned int InternalFormat; // format of texture object
        unsigned int ImageFormat; // format of loaded image

        // texture configuration
        unsigned int WrapS;
        unsigned int WrapT;
        unsigned int FilterMin; // filtering mode if texture pixels < screen pixels
        unsigned int FilterMax; // filtering mode if texture pixels > screen pixels

        void Generate(int width, int height, int bpp, unsigned char* data);
        void Bind(unsigned int slot = 0) const;
        void UnBind() const;
        
        static Mayra::Texture2D* LoadFromFile(const std::string& filepath);

        unsigned int GetRendererID() const { return m_RendererID; }
    private:
        unsigned int m_RendererID;
        std::string m_Filepath;
        unsigned char* m_LocalBuffer;
    };
}

#endif /* texture_h */
