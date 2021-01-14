#ifndef GameObject_h
#define GameObject_h

#include <Mayra.hpp>

#include "Texture2D.hpp"

namespace Mayra
{
    class GameObject
    {
    public:
        GameObject();
        ~GameObject();

        void Update(float deltaTime);
        void Render();

        glm::vec2 Position;
        Mayra::Texture2D* Texture;
    };
}


#endif /* GameObject_h */
