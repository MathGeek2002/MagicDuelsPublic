#pragma once

#include "../ECS/ECS.hpp"
#include "../graphics/Sprite.h"
#include "../../thirdparty/glm/glm.hpp"
#include "ShapeGenerators.h"
#include "../graphics/Shader.h"
#include <vector>
#include "ParticleData.h"

class ParticleSystem : public Component
{
private:
    int max_particles;
    int spawned_particles_numb;
    int number_alive_partilces;
    Sprite * particleSprite;
    int playerId;
    std::string particleTag;
    std::vector< Entity* > particles;

    float * particle_pos;
    float * particle_col;

    unsigned int nFloatVbos;
    unsigned int *sFloatVbos;
    float **floatVbos;
    unsigned int nVbos;
    unsigned int *vbos;
    glm::vec3 currentPos;

public:
    ParticleSystem();
    ParticleSystem(Sprite*, int maxNumb ,int playerId_,const std::string& particleTag_ = "particle");
    ~ParticleSystem();

    void Emmit(int , ShapeGenerator*,glm::vec3);
    void Deactivate(int id);
    void Draw(Shader *);
    void CreateParticle(int n);

    inline void Swap(Entity** ent,Entity** ent2)
    {
        Entity * temp = *ent2;
        *ent2 = *ent;
        *ent = temp;

        int firstId = (*ent)->getComponent<ParticleData>().id;
        int secondId = (*ent2)->getComponent<ParticleData>().id;
        std::swap( particle_pos[(firstId << 1)], particle_pos[(secondId << 1)]);
        std::swap( particle_pos[(firstId << 1) + 1], particle_pos[(secondId << 1) + 1]);
        std::swap( particle_col[(firstId * 3)], particle_col[(secondId * 3)]);
        std::swap( particle_col[(firstId * 3) + 1], particle_col[(secondId * 3) + 1]);
        std::swap( particle_col[(firstId * 3) + 2], particle_col[(secondId * 3) + 2]);

        std::swap( (*ent)->getComponent<ParticleData>().id , (*ent2)->getComponent<ParticleData>().id );
    }  

    inline void UpdatePos(int id, glm::vec3 pos)
    {
        particle_pos[(id << 1)] = pos.x;
        particle_pos[(id << 1) + 1] = pos.y;
    }

    inline void SetPos(glm::vec3 pos) { currentPos = pos; }
    inline glm::vec3 GetPos() { return currentPos; }
};