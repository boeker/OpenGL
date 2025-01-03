#include "mesh.h"

#include <stb_image.h>

std::vector<Texture> loadedTextures;

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setUpMesh();
}

void Mesh::setUpMesh() {
    // generate object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind VAO
    glBindVertexArray(VAO);

    // copy vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);
    
    // copy indices to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);
    
    
    // configure vertex attribute: vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)0);
    // tell OpenGL to use vertex attributes from array
    glEnableVertexAttribArray(0);

    // configure vertex attribute: vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    // tell OpenGL to use vertex attributes from array
    glEnableVertexAttribArray(1);

    // configure vertex attribute: texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, textureCoordinates));
    // tell OpenGL to use vertex attributes from array
    glEnableVertexAttribArray(2);
}

void Mesh::draw(Shader &shader) {
    unsigned int diffuseNumber = 1;
    unsigned int specularNumber = 1;
    
    shader.use();
    for (unsigned int i = 0; i < textures.size(); ++i) {
        // activate texture unit
        glActiveTexture(GL_TEXTURE0 + i);

        std::string textureNumber;
        std::string textureType = textures[i].type;
        
        if (textureType == "texture_diffuse") {
            textureNumber = std::to_string(diffuseNumber++);
        } else if (textureType == "texture_specular") {
            textureNumber = std::to_string(specularNumber++);
        }

        // tell OpenGL that the sampler belongs to texture unit i
        shader.setInt(("material." + textureType + textureNumber).c_str(), i);

        // bind texture to active texture unit
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // unbind VAO
    glBindVertexArray(0);
}

Mesh Mesh::fromAssimpMesh(aiMesh *mesh, const aiScene *scene, std::string &directory) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.textureCoordinates.x = mesh->mTextureCoords[0][i].x;
            vertex.textureCoordinates.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
        }
        
        vertices.push_back(vertex);
    }
    //std::cout << "INFO::MESH " << vertices.size() << " vertices" << std::endl;

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
           indices.push_back(face.mIndices[j]); 
        }
    }
    //std::cout << "INFO::MESH " << indices.size() << " indices" << std::endl;

    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<Texture> diffuseMaps
            = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
    
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps
            = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);
    
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    //std::cout << "INFO::MESH " << textures.size() << " textures" << std::endl;

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Mesh::loadMaterialTextures(aiMaterial *material,
                                                      aiTextureType type,
                                                      std::string typeName,
                                                      std::string &directory) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
        aiString filename;
        material->GetTexture(type, i, &filename);
        std::string path = directory + '/' + std::string(filename.C_Str()); 

        bool alreadyLoaded = false;
        for (unsigned int j = 0; j < loadedTextures.size(); ++j) {
            if (std::strcmp(loadedTextures[j].pathOfFile.data(), path.data()) == 0) {
                alreadyLoaded = true;

                textures.push_back(loadedTextures[j]);
                break;
            }
        }

        if (!alreadyLoaded) {
            Texture texture = Texture::createTextureFromFile(path, typeName);

            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }

    return textures;
}

Mesh Mesh::cubeMesh() {
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 
                                                             
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
                                                             
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                                             
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                                             
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
                                                             
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Vertex *asVertices = (Vertex*)cubeVertices;
    for (int i = 0; i < 36; ++i) {
        vertices.push_back(asVertices[i]);
        indices.push_back(i);
    }

    std::vector<Texture> textures;
    Texture cont = Texture::createTextureFromFile("textures/container2.png", "texture_diffuse");
    Texture contSpecular = Texture::createTextureFromFile("textures/container2_specular.png", "texture_specular");
    textures.push_back(cont);
    textures.push_back(contSpecular);

    return Mesh(vertices, indices, textures);
}

Mesh Mesh::vegetationMesh() {
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 

         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.499f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.499f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 
         0.5f,  0.5f, -0.499f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 

         0.5f,  0.5f, -0.499f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 
        -0.5f,  0.5f, -0.499f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 
        -0.5f, -0.5f, -0.499f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Vertex *asVertices = (Vertex*)cubeVertices;
    for (int i = 0; i < 12; ++i) {
        vertices.push_back(asVertices[i]);
        indices.push_back(i);
    }

    std::vector<Texture> textures;
    Texture grass = Texture::createTextureFromFile("textures/grass.png", "texture_diffuse", true);
    Texture grassSpec = Texture::createTextureFromFile("textures/grass.png", "texture_specular", true);
    textures.push_back(grass);
    textures.push_back(grassSpec);

    return Mesh(vertices, indices, textures);
}
