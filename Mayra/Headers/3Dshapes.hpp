#ifndef _Dshapes_h
#define _Dshapes_h

#include <glm/glm.hpp>
#include <vector>

namespace Mayra
{
    namespace Shapes
    {
        struct Shape
        {
            std::vector<float> vertices;
            int stride;
            
            int getVerticesCount()
            {
                return vertices.size() / stride;
            };
        };
        struct Cube
        {
            float vertices[36 * 6];
            int size;
            int verticesCount;
            int stride;
        };

        struct Pyramid
        {
            float vertices[18 * 6];
            int size;
            int verticesCount;
            int stride;
        };

        struct Quad
        {
            float vertices[6 * 6];
            int size;
            int verticesCount;
            int stride;
        };

        struct Tetrahedron
        {
            float vertices[12 * 6];
            int size;
            int verticesCount;
            int stride;
        };

        static const Mayra::Shapes::Cube cube = {
            {
                // X    Y      Z      Normal
                -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,// triangle 1 : begin
                -0.5f,-0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f, // triangle 1 : end

                0.5f, 0.5f,-0.5f,   0.0f, 0.0f, -1.0f, // triangle 2 : begin
                -0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,
                -0.5f, 0.5f,-0.5f,  0.0f, 0.0f, -1.0f, // triangle 2 : end

                0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,
                -0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,
                0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,

                0.5f, 0.5f,-0.5f,  0.0f, 0.0f, -1.0f,
                0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,
                -0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,

                -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,

                0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,
                -0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,
                -0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,

                -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
                -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
                0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,

                0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
                0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
                0.5f, 0.5f,-0.5f,  1.0f, 0.0f, 0.0f,

                0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
                0.5f,-0.5f, 0.5f,  1.0f, 0.0f, 0.0f,

                0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
                0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
                -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
                0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f
            },
            36 * 6,
            36,
            6
        };

        static const Mayra::Shapes::Cube pyramid = {
            {
                // X    Y      Z      Normal
                0.0f,  0.5f,  0.0f,  0.0f,  0.5f, 1.0f, // A - 0
                -0.5f, -0.5f,  0.5f,  0.0f,  0.5f, 1.0f, // E - 4
                0.5f, -0.5f,  0.5f,  0.0f,  0.5f, 1.0f, // D - 3

                0.0f,  0.5f,  0.0f,  1.0f,  0.5f, 0.0f, // A - 0
                0.5f, -0.5f,  0.5f,  1.0f,  0.5f, 0.0f, // D - 3
                0.5f, -0.5f, -0.5f,  1.0f,  0.5f, 0.0f, // C - 2

                0.0f,  0.5f,  0.0f,  0.0f,  0.5f, -1.0f, // A - 0
                0.5f, -0.5f, -0.5f,  0.0f,  0.5f, -1.0f, // C - 2
                -0.5f, -0.5f, -0.5f,  0.0f,  0.5f, -1.0f, // B - 1

                0.0f,  0.5f,  0.0f, -1.0f,  0.5f, 0.0f, // A - 0
                -0.5f, -0.5f, -0.5f, -1.0f,  0.5f, 0.0f, // B - 1
                -0.5f, -0.5f,  0.5f, -1.0f,  0.5f, 0.0f, // E - 4

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,

                0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f
            },
            18 * 6,
            18,
            6
        };

        static const Mayra::Shapes::Quad quad = {
            {
                // X    Y      Z      Normal
                0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
                0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
            },
            6 * 6,
            6,
            6
        };

        static const Mayra::Shapes::Tetrahedron tetrahedron = {
            {
                -0.471405f, -0.333333f, -0.816497f,   1.08866f,  0.769801f, -1.88562f, // C - 2
                0.000000f,  1.000000f,  0.000000f,   1.08866f,  0.769801f, -1.88562f, // A - 0
                0.942809f, -0.333333f,  0.000000f,   1.08866f,  0.769801f, -1.88562f, // B - 1

                -0.471405f, -0.333333f,  0.816497f,  -2.17732f,  0.769802f,  0.00000f, // D - 3
                0.000000f,  1.000000f,  0.000000f,  -2.17732f,  0.769802f,  0.00000f, // A - 0
                -0.471405f, -0.333333f, -0.816497f,  -2.17732f,  0.769802f,  0.00000f, // C - 2

                -0.471405f, -0.333333f, -0.816497f,   0.00000f, -2.309400f,  0.000000f, // C - 2
                0.942809f, -0.333333f,  0.000000f,   0.00000f, -2.309400f,  0.000000f, // B - 1
                -0.471405f, -0.333333f,  0.816497f,   0.00000f, -2.309400f,  0.000000f, // D - 3

                0.942809f, -0.333333f,  0.000000f,   1.08866f,  0.769801f,  1.88562f, // B - 1
                0.000000f,  1.000000f,  0.000000f,   1.08866f,  0.769801f,  1.88562f, // A - 0
                -0.471405f, -0.333333f,  0.816497f,   1.08866f,  0.769801f,  1.88562f, // D - 3
            },
            12 * 6,
            12,
            6
        };
    }
}

#endif /* _Dshapes_h */
