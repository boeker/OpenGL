#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
private:
    unsigned int textureID;

    int width;
    int height;
    int numberOfChannels;

    void setUpTextureObject();

public:
    Texture();
    void bind();
    void loadFromFile(const std::string &filename);
};

#endif
