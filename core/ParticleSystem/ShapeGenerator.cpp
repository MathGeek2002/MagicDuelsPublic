#include "ShapeGenerators.h"
#include "ParticleData.h"
#include "../Random/Random.hpp"
#include "../Input/Gamepad.h"
#include "../Input/Input.h"

void CircleGenerator::Generate(ParticleData* particleData, glm::vec3 ownerPos,int playerId)
{
    glm::vec2 randDir = Random::RandDirection(0.f,360.f);
    glm::vec3 dir = glm::vec3( randDir.x, randDir.y, 0.f );

    particleData->SetSpace(generateInLocalSpace);
    particleData->SetData(ownerPos + dir * radius, glm::vec3(0), 0);

    particleData->SetLifeTime(particlelifeTime);
}

void ConeGenerator::Generate(ParticleData* particleData, glm::vec3 ownerPos,int playerId)
{
    glm::vec2 randDir = Random::RandDirection(-18.f,18.f);
    glm::vec3 particleStartPos = offset; 

    if( isFlipped ){
        randDir.x = -randDir.x;
        particleStartPos = glm::vec3(-offset.x,offset.y,0.f);
    }

    glm::vec3 vel = glm::vec3(randDir.x, randDir.y, 0.f) * speed; 
    particleStartPos += ownerPos;

    particleData->SetSpace( generateInLocalSpace = false );
    particleData->SetData(particleStartPos, vel, 35);

    particleData->SetLifeTime(particlelifeTime);
}

void LineGenerator::Generate(ParticleData* particleData, glm::vec3 ownerPos,int playerId)
{
    glm::vec3 vel;;

    glm::vec3 direction = glm::vec3( 
        Input::getGamepadAxis(Gamepad::AXIS_RIGHT_Y, playerId), 
        -Input::getGamepadAxis(Gamepad::AXIS_RIGHT_X, playerId),
        0.f
     );

    glm::vec3 particleStartPos = glm::vec3(0);

    glm::vec3 forward = isFlipped ? glm::vec3(-1,0,0) : glm::vec3(1,0,0);

    if( glm::dot(direction, forward) > 0.01f )
    {
        glm::vec3 normalDir = glm::normalize(direction);
        particleStartPos += normalDir * glm::length(offset);
        vel = normalDir * speed;
    }
    else
    {
        particleStartPos = glm::vec3( isFlipped ? -offset.x : offset.x, offset.y, 0.f);
        vel = (isFlipped ? glm::vec3(-1,0,0) : glm::vec3(1,0,0)) * speed; 
    }

    particleStartPos += ownerPos;

    particleStartPos.y += Random::Range(-1.f,1.f) * 0.01f;
    particleStartPos.x += Random::Range(-1.f,1.f) * 0.01f;

    particleData->SetSpace( generateInLocalSpace = false );
    particleData->SetData(particleStartPos, vel, 20);

    particleData->SetLifeTime(particlelifeTime);
}