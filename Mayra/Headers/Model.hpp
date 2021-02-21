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

class Model
{
public:
    Model(std::string path)
    {
        loadModel(path.c_str());
    }
    void Render(Mayra::Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif /* Model_h */
