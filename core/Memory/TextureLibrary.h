#ifndef TEXTURE_LIBRARY_H
#define TEXTURE_LIBRARY_H

#include "../graphics/Image.h"
#include <unordered_map>

class TextureLibrary
{
private:
    static std::unordered_map <const char*, Image*> textureLibrary;

    TextureLibrary() {}
    
    static TextureLibrary * instance;

public:
    ~TextureLibrary();
    static Image* GetTexture(const char * texturePath);

    static TextureLibrary * GetInstance() { return instance; }
};

#endif