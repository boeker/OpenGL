#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

struct Texture {
    unsigned int id;
    std::string type;
    std::string pathOfFile;

    static unsigned int createTextureIDFromFile(const std::string &path);
    static Texture createTextureFromFile(const std::string &path, const std::string &type);
};

#endif
