//
//  Model.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 16/02/2021.
//

#ifndef Model_h
#define Model_h

#include <Mayra.hpp>
#include <Shader.hpp>
#include "Mesh.hpp"

struct Transform
{
    glm::vec3 Position;
    glm::vec3 Scale;
};

class Model
{
public:
    glm::mat4 object2world;
    Transform parentTransform;
    
    Model(std::string path)
        : object2world(glm::mat4(1.0f))
    {
        loadModel(path.c_str());
    }
    void Render(Mayra::Shader &shader);
    void RenderBBox(Mayra::Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    BoundingBoxProps calculateBoundingBox();

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif /* Model_h */
