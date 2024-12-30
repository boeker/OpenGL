#include "texture.h"

#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

#include "gl.h"

unsigned int Texture::createTextureIDFromFile(const std::string &path, bool alpha) {
    std::cout << "INFO::MESH Loading texture " << path << std::endl;

    // generate texture
    unsigned int textureID;
    glGenTextures(1, &textureID);

    glCheckError();

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    glCheckError();

    // set the texture wrapping/filtering options (on the currently bound texture object)
    if (alpha) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glCheckError();

    int width, height, numberOfChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &numberOfChannels, 0);

    if (data) {
        std::cout << "INFO::TEXTURE::NUMBER_OF_CHANNELS " << numberOfChannels << std::endl;

        GLenum format;
        if (numberOfChannels == 1) {
            format = GL_RED;

        } else if (numberOfChannels == 3) {
            format = GL_RGB;

        } else if (numberOfChannels == 4) {
            format = GL_RGBA;
        }
    
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    stbi_image_free(data);

    return textureID;
}

Texture Texture::createTextureFromFile(const std::string &path, const std::string &type, bool alpha) {
    Texture texture;
    texture.id = createTextureIDFromFile(path, alpha);
    texture.type = type;
    texture.pathOfFile = path;

    return texture;
}

