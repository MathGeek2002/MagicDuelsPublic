#ifndef TEXTURE_SPRITE_H
#define TEXTURE_SPRITE_H

#include "Sprite.h"

class TextureSprite : public Sprite
{
private:
    int w, h;

public:
    TextureSprite(
        float dx = 1.f, float dy = 1.f,
        int _w = 256, int _h = 256);

    void UpdateTexture(GLubyte *tex_);

protected:
    void createTexture();
};

#endif