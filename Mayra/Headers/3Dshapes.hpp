#ifndef _Dshapes_h
#define _Dshapes_h

#include <glm/glm.hpp>
#include <vector>

#define VERTICES_STRIDE 8

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
            float vertices[36 * VERTICES_STRIDE];
            int size;
            int verticesCount;
            int stride;
        };

        struct Pyramid
        {
            float vertices[18 * VERTICES_STRIDE];
            int size;
            int verticesCount;
            int stride;
        };

        struct Quad
        {
            float vertices[6 * VERTICES_STRIDE];
            int size;
            int verticesCount;
            int stride;
        };

        struct Tetrahedron
        {
            float vertices[12 * VERTICES_STRIDE];
            int size;
            int verticesCount;
            int stride;
        };

        static const Mayra::Shapes::Cube cube = {
            {
                // X    Y      Z      Normal           // texture coords
                -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                -0.5f,-0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

                0.5f, 0.5f,-0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
                -0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
                -0.5f, 0.5f,-0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,

                0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
                -0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
                0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,

                0.5f, 0.5f,-0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
                0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
                -0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

                -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

                0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
                -0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
                -0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
                -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
                0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

                0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                0.5f, 0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

                0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                0.5f,-0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

                0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
                0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

                0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

                0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
                0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f
            },
            36 * VERTICES_STRIDE,
            36,
            VERTICES_STRIDE
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
            18 * VERTICES_STRIDE,
            18,
            VERTICES_STRIDE
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
            6 * VERTICES_STRIDE,
            6,
            VERTICES_STRIDE
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
            12 * VERTICES_STRIDE,
            12,
            VERTICES_STRIDE
        };
    }
}

#endif /* _Dshapes_h */
