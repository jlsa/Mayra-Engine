#ifndef TextureCubemap_h
#define TextureCubemap_h

#include <glad/glad.h>
#include <GLUT/glut.h>

#include <Mayra.hpp>

namespace Mayra
{
    class TextureCubemap
    {
    public:
        TextureCubemap();
        ~TextureCubemap();

        int m_Width;
        int m_Height;
        int m_BPP;

        unsigned int InternalFormat;
        unsigned int ImageFormat;

        unsigned int WrapS;
        unsigned int WrapT;
        unsigned int WrapR;
        unsigned int FilterMin;
        unsigned int FilterMax;

//        void Generate(int width, int height, int bpp, unsigned char* data, int index = 0);
        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

//        static Mayra::TextureCubemap* LoadFromFiles(std::vector<std::string> faceFilepaths, bool flip = true);

        unsigned int GetRendererID() const { return m_RendererID; }

        static unsigned int LoadCubemap(std::vector<std::string> faces);
        static Mayra::TextureCubemap* LoadFromFiles(std::vector<std::string> faces);
    private:
        unsigned int m_RendererID;
        std::vector<std::string> m_FaceFilepaths;
        std::vector<unsigned char*> m_LocalBuffers;
//        unsigned char* m_LocalBuffer[6];
    };
}

#endif /* TextureCubemap_h */
