#include "mesh.h"

#include "stb_image.h"

std::vector<TextureStruct> loadedTextures;

Mesh::Mesh(std::vector<VertexStruct> vertices,
           std::vector<unsigned int> indices,
           std::vector<TextureStruct> textures) {
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

    // copy vertices to VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexStruct),
                 &vertices[0], GL_STATIC_DRAW);
    
    // copy indices to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);
    
    
    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normals 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct),
                          (void*)offsetof(VertexStruct, normal));
    glEnableVertexAttribArray(1);

    // vertex texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStruct),
                          (void*)offsetof(VertexStruct, textureCoordinates));
    glEnableVertexAttribArray(2);
}

void Mesh::draw(Shader &shader) {
    unsigned int diffuseNumber = 1;
    unsigned int specularNumber = 1;
    
    for (unsigned int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string textureNumber;
        std::string textureType = textures[i].type;
        
        if (textureType == "texture_diffuse") {
            textureNumber = std::to_string(diffuseNumber++);
        } else if (textureType == "texture_specular") {
            textureNumber = std::to_string(specularNumber++);
        }

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
    std::vector<VertexStruct> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureStruct> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        VertexStruct vertex;

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
        
        std::vector<TextureStruct> diffuseMaps
            = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
    
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<TextureStruct> specularMaps
            = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", directory);
    
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    //std::cout << "INFO::MESH " << textures.size() << " textures" << std::endl;

    return Mesh(vertices, indices, textures);
}

std::vector<TextureStruct> Mesh::loadMaterialTextures(aiMaterial *material,
                                                      aiTextureType type,
                                                      std::string typeName,
                                                      std::string &directory) {
    std::vector<TextureStruct> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
        aiString path;
        material->GetTexture(type, i, &path);
        bool alreadyLoaded = false;
        for (unsigned int j = 0; j < loadedTextures.size(); ++j) {
            if (std::strcmp(loadedTextures[j].pathOfFile.data(), path.C_Str()) == 0) {
                alreadyLoaded = true;

                textures.push_back(loadedTextures[j]);
                break;
            }
        }

        if (!alreadyLoaded) {
            TextureStruct texture;
            texture.id = createTextureFromFile(path.C_Str(), directory);
            texture.type = typeName;
            texture.pathOfFile = path.C_Str();

            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }

    return textures;
}

unsigned int Mesh::createTextureFromFile(const char *path, const std::string &directory) {
    std::string filename(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, numberOfChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &numberOfChannels, 0);
    
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}
