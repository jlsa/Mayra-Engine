#ifndef Skybox_h
#define Skybox_h

#include <Mayra.hpp>
#include "TextureCubemap.h"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include <3Dshapes.hpp>
#include "Camera.hpp"

namespace Mayra
{
    class Skybox
    {
    public:
        Skybox();
        ~Skybox();

        void Render(Camera* camera);

    private:
        TextureCubemap* m_Texture;
        Shader* m_Shader;
        VertexArray* m_VertexArray;
        VertexBuffer* m_VertexBuffer;
    };
}

#endif /* Skybox_h */
