#include "TextureSprite.h"

TextureSprite::TextureSprite(
    float dx_, float dy_,
    int w_, int h_) : Sprite(dx_, dy_, nullptr), w(w_), h(h_)
{
    createTexture();
}

void TextureSprite::createTexture()
{
    GLubyte *tex = new GLubyte[4 * w * h];

    for (int i = 0; i < w * h; i++)
    {
        tex[i * 4 + 0] = 255;
        tex[i * 4 + 1] = 255;
        tex[i * 4 + 2] = 255;
        tex[i * 4 + 3] = 255;
    }

    //glGenTextures(1, &texture); //texture generated in parent constructor
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);

    delete[] tex;
}

void TextureSprite::UpdateTexture(GLubyte *tex_)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_);
}