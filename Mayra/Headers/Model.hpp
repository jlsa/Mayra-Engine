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

namespace Mayra
{
    struct BoneInfo
    {
        // id is index in finalBoneMatrices
        int id;
        // offset matrix transforms vertex from model space to bone space
        glm::mat4 offset;
    };

    struct Transform
    {
        glm::vec3 Position;
        glm::vec3 Scale;
    };

    class Model
    {
    public:
        glm::mat4 object2world;

        Model(std::string path)
            : object2world(glm::mat4(1.0f))
        {
            loadModel(path.c_str());
        }

        void Render(Mayra::Shader* shader);
        void RenderBBox(Mayra::Shader* shader);

        std::string m_Path;

        std::map<std::string, BoneInfo>& GetOffsetMatMap() { return m_OffsetMatMap; }
        int& GetBoneCount() { return m_BoneCount; }

    private:
        std::vector<Mayra::Mesh> meshes;
        std::string directory;
        std::vector<Mayra::Texture> textures_loaded;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Mayra::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

        std::map<std::string, BoneInfo> m_OffsetMatMap;
        int m_BoneCount = 0;

        void SetVertexBoneDataToDefault(Vertex& vertex);
        void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
        void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    };
}

#endif /* Model_h */
