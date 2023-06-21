#include "AnimatedSprite.h"
#include "../Memory/TextureLibrary.h"
#include <SDL2/SDL.h>

AnimatedSprite::AnimatedSprite(float dx_, float dy_) : Sprite(dx_, dy_, "ASSETS/checkerboard.png", animated_sprite_T)
{
    curImage = nullptr;
    nFrames = 1;
    speed = 1;
    previousTicks = 0;
    loopAnimation = true;
}

void AnimatedSprite::setScale(float x, float y)
{
    Sprite::setScale(x * nFrames, y);
}

void AnimatedSprite::Draw(Shader *shader_)
{
    Animate();
    Sprite::Draw(shader_);
}

void AnimatedSprite::Animate()
{
    if( !loopAnimation && ((currentFrame == nFrames - 1 && flipH) || (currentFrame == nFrames - 2  && !flipH)) ) //TODO fix flip in shader instead
        return;

    int previousFrame = currentFrame;

    size_t currentTicks = SDL_GetTicks();
    ticks += currentTicks - previousTicks;

    currentFrame = (static_cast<int>(ticks / speed)) % nFrames;

    previousTicks = currentTicks;

    if (previousFrame != currentFrame)
    {
        offsetX = (1.f / nFrames) * (float)(currentFrame + 1); //DOTO fix shader flip and remove this +1
    }
}

bool AnimatedSprite::addAnimation(const char *name, const char *texturePath, int nFrames, int speed, bool loop_)
{
    int count = animations.count(name);
    if (count > 0)
    {
        return false;
    }

    Image *image = TextureLibrary::GetTexture(texturePath);
    animationData animData{image, nFrames, speed, loop_};
    animations[name] = animData;

    if (curImage == nullptr)
    {
        setAnimation(name);
    }

    return true;
}

bool AnimatedSprite::setAnimation(const char *name)
{
    int count = animations.count(name);
    if (count == 0)
    {
        return false;
    }

    animationData *temp = &animations[name];

    currentFrame = -1;
    ticks = 0;
    nFrames = temp->nFrames;
    speed = temp->speed;
    loopAnimation = temp->loop;
    if (curImage != temp->image)
    {
        curImage = temp->image;
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->image->w, temp->image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, temp->image->tex);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    setScale(1, 1);

    return true;
}

float AnimatedSprite::GetAnimationProcentage()
{
    return (float)(currentFrame + 1) / nFrames;
}