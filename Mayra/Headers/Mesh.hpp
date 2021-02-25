//
//  Mesh.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 16/02/2021.
//

#ifndef Mesh_h
#define Mesh_h

#include <Mayra.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.hpp>
#include <GL.hpp>

//GLint attribute_v_coord = -1;

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
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;
    glm::mat4 object2world;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
        : vertices(vertices), indices(indices), textures(textures), object2world(glm::mat4(1.0f))
    {
        setupMesh();
    }

    void Render(Mayra::Shader &shader) const
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        unsigned int emissionNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].Type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);
            else if (name == "texture_emission")
                number = std::to_string(emissionNr++);

            // shader.SetInt(("material." + name + number).c_str(), i);
            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.GetRendererID(), (name + number).c_str()), i);

            glBindTexture(GL_TEXTURE_2D, textures[i].ID);
        }

        // render mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

    void RenderBBox(Mayra::Shader &shader) const
    {
        if (this->vertices.size() == 0)
            return;

        GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f, 1.0f,
             0.5f, -0.5f, -0.5f, 1.0f,
             0.5f,  0.5f, -0.5f, 1.0f,
            -0.5f,  0.5f, -0.5f, 1.0f,
            -0.5f, -0.5f,  0.5f, 1.0f,
             0.5f, -0.5f,  0.5f, 1.0f,
             0.5f,  0.5f,  0.5f, 1.0f,
            -0.5f,  0.5f,  0.5f, 1.0f
        };
        GLuint vbo_vertices;
        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLushort elements[] = {
            0, 1, 2, 3,
            4, 5, 6, 7,
            0, 4, 1, 5, 2, 6, 3, 7
        };

        GLuint ibo_elements;
        glGenBuffers(1, &ibo_elements);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        float min_x, max_x;
        float min_y, max_y;
        float min_z, max_z;

        min_x = max_x = this->vertices[0].Position.x;
        min_y = max_y = this->vertices[0].Position.y;
        min_z = max_z = this->vertices[0].Position.z;

        for (unsigned int i = 0; i < this->vertices.size(); i++) {
            glm::vec3 position = this->vertices[i].Position;

            if (position.x < min_x) min_x = position.x;
            if (position.x > max_x) max_x = position.x;

            if (position.y < min_y) min_y = position.y;
            if (position.y > max_y) max_y = position.y;

            if (position.z < min_z) min_z = position.z;
            if (position.z > max_z) max_z = position.z;
        }

        glm::vec3 size = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
        glm::vec3 center = glm::vec3((min_x + max_x) / 2,
                                     (min_y + max_y) / 2,
                                     (min_z + max_z) / 2);
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, size);
        transform = glm::translate(transform, center);

        // apply object's transformation matrix
        glm::mat4 m = this->object2world * transform;
        shader.SetMat4("m", m);
//        glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(m));

        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                              0,                    // attribute
                              4,                    // number of elements per vertex, here (x,y,z,w)
                              GL_FLOAT,             // the type of each element
                              GL_FALSE,             // take our values as-is
                              0,                    // no extra data between each position
                              0                     // offset of first element
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
        glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo_vertices);
        glDeleteBuffers(1, &ibo_elements);
    }

private:
    unsigned int VBO, EBO;

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }
};


#endif /* Mesh_h */
