#include "model.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <queue>

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


Model::Model(Texture &texture, Shader *shader) {
    this->texture = texture;
    this->shader = shader;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Model::setGeometry(float *vertices, unsigned int num) {
    this->vertices = vertices;
    this->numOfVertices = num;
}

void Model::transferGeometry() {
    // bind
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, numOfVertices * 5 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::draw() {
    shader->use();
    shader->setInt("textureSampler", 0);
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}


Shader* Model::getShader() {
    return shader;
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
