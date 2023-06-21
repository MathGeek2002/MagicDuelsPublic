#include "TextureLibrary.h"
#include <iostream>

TextureLibrary * TextureLibrary::instance;

std::unordered_map <const char*, Image*> TextureLibrary::textureLibrary;

TextureLibrary::~TextureLibrary()
{
    for(auto it = textureLibrary.begin(); it != textureLibrary.end(); it++)
        delete (it->second);
}

Image* TextureLibrary::GetTexture(const char * texturePath)
{
    if(textureLibrary.find(texturePath) == textureLibrary.end())
    {
        Image * newImage = new Image(texturePath);
        textureLibrary[texturePath] = newImage;
        return newImage;
    }
    else
    {
        return textureLibrary[texturePath];
    }
}