#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include "mesh.h"
#include "shader.h"
#include "texture.h"

class Model {
private:
    std::string directory;
    std::vector<Mesh> meshes;

    void processScene(const aiScene *scene, std::string &directory);

public:
    Model();
    Model(const Mesh &mesh);
    void loadModel(const std::string &path);
    void draw(Shader &shader);
};

#endif
