#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include "../../thirdparty/glm/glm.hpp"

class ParticleData;

class ShapeGenerator
{   
protected:
    float particlelifeTime;
    glm::vec3 offset;
    bool isFlipped;
    bool generateInLocalSpace;

public:

    ShapeGenerator() { }
    virtual ~ShapeGenerator() { }

    virtual inline void SetFilpped(bool isFlipped_) { isFlipped = isFlipped_; }

    virtual void Generate(ParticleData* particleData,glm::vec3 startPos, int playerId) = 0;
};

class CircleGenerator : public ShapeGenerator
{
private:
    float radius;

public:
    CircleGenerator(float radius_,float lifeTime_ = 0.1f, bool drawInLocalSpace = false) : radius(radius_)
    {
        particlelifeTime = lifeTime_;
        offset = glm::vec3(0);
        generateInLocalSpace = drawInLocalSpace;
    }

    ~CircleGenerator() { }

    void SetFilpped(bool isFlipped_) override
    {
        ShapeGenerator::SetFilpped(isFlipped_);
    }

    void Generate(ParticleData* particleData, glm::vec3 ownerPos, int playerId) override;
};

class ConeGenerator : public ShapeGenerator
{
private:
    float speed;
    bool isFlipped;

public:
    ConeGenerator(float speed_,float lifeTime_ = 2.f,glm::vec3 offset_ = glm::vec3(0.1f,0.05f,0.f)) : speed(speed_) 
    {
        offset = offset_;
        particlelifeTime = lifeTime_;
        isFlipped = false;
        generateInLocalSpace = false;
    }

    void SetFilpped(bool isFlipped_) override
    {
        isFlipped = isFlipped_;
    }

    ~ConeGenerator() {  }

    void Generate(ParticleData* particleData, glm::vec3 ownerPos,int playerId) override;
};

class LineGenerator : public ShapeGenerator
{
private:
    float speed;
    bool isFlipped;
    float width;

public:
    LineGenerator(float speed_,float width_,float lifeTime_ = 2.f,glm::vec3 offset_ = glm::vec3(0.1f,0.05f,0.f)) : speed(speed_),width(width_) 
    {
        offset = offset_;
        particlelifeTime = lifeTime_;
        isFlipped = false;
        generateInLocalSpace = false;
    }

    void SetFilpped(bool isFlipped_) override
    {
        isFlipped = isFlipped_;
    }

    ~LineGenerator() {};

    void Generate(ParticleData* particleData, glm::vec3 ownerPos, int playerId) override;
};

#endif