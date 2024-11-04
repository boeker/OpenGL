#include "texture.h"

#include <iostream>
#include "glad/glad.h"
#include "stb_image.h"

void Texture::setUpTextureObject() {
    // bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // unbind texture object
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture() {
    glGenTextures(1, &textureID);
    setUpTextureObject();
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::loadFromFile(const std::string &filename) {
    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &numberOfChannels, 0);
    
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}
