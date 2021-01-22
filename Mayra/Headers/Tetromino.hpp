#ifndef Tetromino_h
#define Tetromino_h

#include <Mayra.hpp>
#include "GameObject.hpp"
#include "OrthographicCamera.hpp"
#include "Sprite.hpp"

namespace Mayra
{
    enum Shape
    {
        Oo,
        Ii,
        Ss,
        Zz,
        Ll,
        Jj,
        Tt,
        NUM_OF_SHAPES
    };
    const bool shapes[28][4] =
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},

        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},

        {0, 0, 0, 0},
        {0, 0, 1, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 0},

        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 0},

        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 1, 0, 0},
        {0, 0, 0, 0},

        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 0},

        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    };

    class Tetromino : public Mayra::GameObject
    {
    public:
        Tetromino();
        ~Tetromino();

        void Render(Mayra::OrthographicCamera* camera);
        void Update(float deltaTime);


        //        void Left();
        //        void Right();
        //        void Rotate();
        //        void Up();
        //        void Down();

    private:
        Mayra::Shape m_Shape;
//        bool m_Blocks[4][4];
        glm::vec4 m_Color;

        
        Sprite* m_Tetromino0;
        Sprite* m_Tetromino1;
        Sprite* m_Tetromino2;
        Sprite* m_Tetromino3;
    };
}

#endif /* Tetromino_h */
