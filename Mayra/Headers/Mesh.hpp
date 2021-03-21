#ifndef Mesh_h
#define Mesh_h

#include <Mayra.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL.hpp>

#include "Shader.hpp"

//GLint attribute_v_coord = -1;

namespace Mayra
    {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture {
        unsigned int ID;
        std::string Type;
        std::string Path;
    };

    class Mesh
    {
    public:
        std::vector<Mayra::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Mayra::Texture> textures;
        unsigned int VAO;
        glm::mat4 object2world;

        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

        void Render(Mayra::Shader *shader) const;

        void RenderBBox(Mayra::Shader* shader) const;

    private:
        unsigned int VBO, EBO;

        void setupMesh();
    };
}

#endif /* Mesh_h */
