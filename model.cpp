#include "model.h"

#include <queue>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void Model::processScene(const aiScene *scene, std::string &directory) {
    std::queue<aiNode*> nodes;
    nodes.push(scene->mRootNode);

    while (!nodes.empty()) {
        aiNode *node = nodes.front();
        nodes.pop();

        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            // process mesh
            meshes.push_back(Mesh::fromAssimpMesh(mesh, scene, directory));
        }
        
        if (node ->mNumChildren > 0) {
            std::cout << "INFO::MODEL Node has " << node->mNumChildren << " children" << std::endl;
        }
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            nodes.push(node->mChildren[i]);
        }
    }
}

Model::Model() {
}

Model::Model(const Mesh &mesh) {
    meshes.push_back(mesh);
}

void Model::loadModel(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP" << import.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processScene(scene, directory);
}

void Model::draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); ++i) {
        meshes[i].draw(shader);
    }
}
