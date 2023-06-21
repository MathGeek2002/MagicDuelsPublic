#include "ParticleSystem.h"
#include "../Application.h"
#include "../graphics/GraphicsComponent.h"
#include "../TransformComponent/TransformComponent.h"
//#include "../Debug/Debug.hpp"
#include "../graphics/RenderEngine.h"
#include "../Random/Random.hpp"

ParticleSystem::ParticleSystem(Sprite* sprite, int maxNumb, int playerId_,const std::string& particleTag_) : Component(CT_PARTICLE_SYSTEM),
max_particles(maxNumb),
particleSprite(sprite),
playerId(playerId_),
particleTag(particleTag_)
{
    number_alive_partilces = 0;
    spawned_particles_numb = 0;

    particle_pos = new float[max_particles * 2];
    particle_col = new float[max_particles * 3];

    int start_spawn_numb = std::min(2000, max_particles);

    for(int i = 0;i < start_spawn_numb / 10; i++)
        CreateParticle(10);

    RenderEngine::RegisterParticleSystem(this);

    nFloatVbos = 2;
    sFloatVbos = new unsigned int[nFloatVbos];
    sFloatVbos[0] = 2;
    sFloatVbos[1] = 3;
    floatVbos = new float* [nFloatVbos];
    floatVbos[0] = particle_pos;
    floatVbos[1] = particle_col;

    nVbos = nFloatVbos;
    vbos = new unsigned int [nVbos];
    glGenBuffers(nVbos, vbos);

    glBindVertexArray( particleSprite->Get_VAOS() );

    for(size_t i = 0;i < nFloatVbos; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glEnableVertexAttribArray( 4 + i );
        glVertexAttribPointer( 4 + i , sFloatVbos[i], GL_FLOAT, GL_FALSE, sFloatVbos[i] * sizeof(float), (void*)0 );
        glVertexAttribDivisor( 4 + i , 1);
    }
    glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{ 
    RenderEngine::UnRegisterParticleSystem(this);
    delete [] particle_pos;
    delete [] particle_col;
    delete [] sFloatVbos;
    delete [] floatVbos;
}

void ParticleSystem::Emmit(int n_particles, ShapeGenerator* myGenerator,glm::vec3 startPos)
{
    int from = number_alive_partilces;
    int to = std::min(from + n_particles, max_particles);

    for(int i = from; i < to; i++)
    {
        if(i >= spawned_particles_numb)
            CreateParticle(10);

        particles[i]->getComponent<ParticleData>(); 
        
        myGenerator->Generate(&particles[i]->getComponent<ParticleData>(), startPos, playerId);
        number_alive_partilces++;
    }
}

void ParticleSystem::Deactivate(int id)
{
    number_alive_partilces--;

    if(number_alive_partilces > 0)
        Swap( &particles[id], &particles[number_alive_partilces]);
}

void ParticleSystem::Draw(Shader * shader)
{
    for( int i = 0; i < (int)nFloatVbos; i++ )
    {
      glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
      glBufferData(
        GL_ARRAY_BUFFER,
        number_alive_partilces * sFloatVbos[i] * sizeof(float),
	      floatVbos[i], GL_DYNAMIC_DRAW
      );
    }

    glm::mat4 modelM = glm::mat4(1.f);

    glUniformMatrix4fv( glGetUniformLocation( shader->program, "model"), 1, GL_FALSE, glm::value_ptr(modelM));

    glBindVertexArray(particleSprite->Get_VAOS());

    glDrawElementsInstanced(GL_TRIANGLES, particleSprite->GetNumberOfInfices(), GL_UNSIGNED_INT, 0, number_alive_partilces);
}

void ParticleSystem::CreateParticle(int n)
{   
    for(int i = spawned_particles_numb;i < std::min(spawned_particles_numb + n, max_particles); i++ )
    {
        particles.emplace_back( new Entity() );
        Application::Register(particles[spawned_particles_numb]);

        particles[spawned_particles_numb]->addComponent<TransformComponent>( glm::vec3(0) );

        int id = spawned_particles_numb;

        if( particleTag == "orange" )
        {
            particle_col[id * 3] = 1.f;
            particle_col[id * 3 + 1] = Random::Range(0.f,0.7f);
            particle_col[id * 3 + 2] = Random::Range(0.f,0.2f);
        }
        else if( particleTag == "blue" )
        {
            particle_col[id * 3] = Random::Range(0.f,0.2f);
            particle_col[id * 3 + 1] = Random::Range(0.f,0.7f);
            particle_col[id * 3 + 2] = 1.f;
        }
        else
        {
            particle_col[id * 3] = Random::Range(0.7f,1.f);
            particle_col[id * 3 + 1] = Random::Range(0.7f,1.f);
            particle_col[id * 3 + 2] = 0.f;
        }

        particles[spawned_particles_numb]->tag = particleTag;
        particles[spawned_particles_numb]->addComponent<ParticleData>( spawned_particles_numb, this );
        spawned_particles_numb++;
    }
}
