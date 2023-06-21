#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "Sprite.h"
#include "Image.h"
#include <map>

class AnimatedSprite : public Sprite
{
private:
    struct animationData
    {
        Image *image;
        int nFrames;
        int speed;
        bool loop;
    };
    std::map<std::string, animationData> animations;

public:
    AnimatedSprite(float dx_ = 1.f, float dy_ = 1.f);
    bool addAnimation(const char *name, const char *texturePath, int nFrames, int speed,bool loop_ = true);
    bool setAnimation(const char *name);
    void Animate();
    void setScale(float, float);
    void Draw(Shader *shader_) override;

    float GetAnimationProcentage();

protected:
    int nFrames;
    int speed;
    int currentFrame;
    Image *curImage;
    size_t previousTicks;
    size_t ticks;
    bool loopAnimation;
};

#endif