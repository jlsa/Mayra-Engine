#include "Model.hpp"

#include <iostream>
#include "3Dshapes.hpp"

void Model::Render(Mayra::Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Render(shader);
    }
}

void Model::RenderBBox(Mayra::Shader &shader)
{
    if (meshes.size() == 0)
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

    GLushort elements[] = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    unsigned int index = 0;
    {
        unsigned int lightVBO, lightCubeVAO;
        glGenBuffers(1, &lightVBO);

        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);

        // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        unsigned int ibo_elements;
        glGenBuffers(1, &ibo_elements);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        float min_x, max_x;
        float min_y, max_y;
        float min_z, max_z;

        min_x = max_x = meshes[index].vertices[0].Position.x;
        min_y = max_y = meshes[index].vertices[0].Position.y;
        min_z = max_z = meshes[index].vertices[0].Position.z;

        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            for (unsigned int j = 0; j < meshes[i].vertices.size(); j++)
            {
                glm::vec3 position = meshes[i].vertices[j].Position;

                if (position.x < min_x) min_x = position.x;
                if (position.x > max_x) max_x = position.x;

                if (position.y < min_y) min_y = position.y;
                if (position.y > max_y) max_y = position.y;

                if (position.z < min_z) min_z = position.z;
                if (position.z > max_z) max_z = position.z;
            }
        }

        glm::vec3 size = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);

        glm::vec3 center = glm::vec3((min_x + max_x) / 2,
                                     (min_y + max_y) / 2,
                                     (min_z + max_z) / 2);

        glm::mat4 transform = glm::mat4(1.0f);

        transform = glm::translate(transform, center);
        transform = glm::scale(transform, size);
        //
        // apply object's transformation matrix
        glm::mat4 m = object2world * transform;
        shader.SetMat4("model", m);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
        glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &lightVBO);
        glDeleteBuffers(1, &ibo_elements);
    }
}

void Model::loadModel(std::string path)
{
    std::cout << path << std::endl;
    Assimp::Importer importer;
    unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
    const aiScene* scene = importer.ReadFile(path, flags);

    if (scene) {
        std::cout << "scene exists" << std::endl;
    }
    else {
        std::cout << "scene does NOT exists" << std::endl;
    }

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    // recusively process ASSIMP's root node
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vec3;
        // positions
        vec3.x = mesh->mVertices[i].x;
        vec3.y = mesh->mVertices[i].y;
        vec3.z = mesh->mVertices[i].z;
        vertex.Position = vec3;

        // normals
        if (mesh->HasNormals())
        {
            vec3.x = mesh->mNormals[i].x;
            vec3.y = mesh->mNormals[i].y;
            vec3.z = mesh->mNormals[i].z;
            vertex.Normal = vec3;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec2;
            // a vertex can contain up to 8 different texture coordinates. Only using 1 here.
            vec2.x = mesh->mTextureCoords[0][i].x;
            vec2.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec2;

            // tangent
            vec3.x = mesh->mTangents[i].x;
            vec3.y = mesh->mTangents[i].y;
            vec3.z = mesh->mTangents[i].z;
            vertex.Tangent = vec3;

            // bitangent
            vec3.x = mesh->mBitangents[i].x;
            vec3.y = mesh->mBitangents[i].y;
            vec3.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vec3;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    // handle indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // handle materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // diffuse
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // specular
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // normal
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // height
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

unsigned int TextureFromFile(char const *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else return textureID;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].Path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture.ID = TextureFromFile(str.C_Str(), this->directory);
            texture.Type = typeName;
            texture.Path = str.C_Str();

            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}


