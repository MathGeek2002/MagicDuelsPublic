#pragma once

#include "../ECS/ECS.hpp"
#include "ParticleSystem.h"
#include "vector"
#include "../graphics/Sprite.h"
#include "ShapeGenerators.h"
#include <iostream>
#include "../Input/Input.h"
#include <SDL2/SDL.h>
#include "../graphics/GraphicsComponent.h"
#include "../TransformComponent/TransformComponent.h"
#include "../Input/Gamepad.h"
#include "../../game/Player/PlayerMana.hpp"
//#include "../Debug/Debug.hpp"

enum ParticleType
{
    PT_ORANGE,
    PT_BLUE,
    PT_SHIELD
};

enum EmmiterShape
{
    CIRCLE_SHAPE,
    CONE_SHAPE,
    LINE_SHAPE
};

class ParticleManager : public Component
{
private:
    std::vector < ParticleSystem* > particleSystems;
    ParticleType currentType;
    Sprite * sprite_orange;
    Sprite * sprite_blue;
    Sprite * sprite_shield;

    ShapeGenerator * currentGenerator;
    CircleGenerator * circleGenerator;
    ConeGenerator * coneGenerator;
    LineGenerator * lineGenerator;

    int emmitRate = 20;
    float manaCost = 15.f;
    EmmiterShape currentShape;

    Uint8 myGamepadId;

    const int TRIGGER_DEADZONE = 20000;

public:

    ParticleManager(Uint8 gamepadId_ ) : Component(CT_CUSTOM), myGamepadId(gamepadId_)
    {
        circleGenerator = new CircleGenerator(0.2f, 0.1f, true);
        currentGenerator = circleGenerator;
        currentShape = CONE_SHAPE;

        coneGenerator = new ConeGenerator(1.3f,1.5f, glm::vec3(0.06f,0.f,0.f));

        lineGenerator = new LineGenerator(1.7f, 0.02f, 1.f, glm::vec3(0.12f,-0.03f,0.f));

        currentType = PT_ORANGE;
        
        sprite_orange = new Sprite(0.02f,0.02f,nullptr,sprite_T);
        sprite_orange->setColor( 1.f, 0.3f,0.f );

        particleSystems.push_back( new ParticleSystem(sprite_orange, 500, gamepadId_, "orange"));

        sprite_blue = new Sprite(0.02f,0.02f,nullptr,sprite_T);
        sprite_blue->setColor( 0.1f, 0.2f,0.9f );

        particleSystems.push_back( new ParticleSystem(sprite_blue, 500, gamepadId_, "blue") );

        sprite_shield = new Sprite(0.02f, 0.02f, nullptr, sprite_T);
        sprite_shield->setColor(1.f,1.f,1.f);

        particleSystems.push_back( new ParticleSystem( sprite_shield, 400, gamepadId_, "shield") );
    }

    void init() override
    {
        particleSystems[0]->SetPos( entity->getComponent<TransformComponent>().position );
        particleSystems[1]->SetPos( entity->getComponent<TransformComponent>().position );
        particleSystems[2]->SetPos( entity->getComponent<TransformComponent>().position );
    }

    ~ParticleManager()
    {
        delete circleGenerator;
        delete coneGenerator;
        delete lineGenerator;

        delete particleSystems[0];
        delete particleSystems[1];
        delete particleSystems[2];

        delete sprite_orange;
        delete sprite_blue;
        delete sprite_shield;
    }

    void Emmit(int numb)
    {
        particleSystems[(int)currentType] -> Emmit(numb, currentGenerator, entity->getComponent<TransformComponent>().position);
    }

    inline void ChangeParticleType(ParticleType newType)
    {
        currentType = newType;
    }

    void ChangeShape(EmmiterShape newShape)
    {
        currentShape = newShape;

        switch (newShape)
        {
        case CIRCLE_SHAPE:
            currentGenerator = circleGenerator;
            ChangeParticleType(PT_SHIELD);
            emmitRate = 70;
            manaCost = 12.f;
            break;
        
        case CONE_SHAPE:
            currentGenerator = coneGenerator;
            emmitRate = 10;
            manaCost = (float) emmitRate * 0.5f;
            break;
        
        case LINE_SHAPE:
            currentGenerator = lineGenerator;
            emmitRate = 5;
            manaCost = 6.f;
            break;

        default:
            currentGenerator = coneGenerator;
            emmitRate = 10;
            manaCost = (float) emmitRate * 0.5f;
            break;
        }
    }

    inline void SetEmmitRate(int newEmmitRate) {  emmitRate = newEmmitRate;  }


    void run(float dt)
    {
        if( entity->hasComponent<GraphicsComponent>() )
            currentGenerator->SetFilpped( entity->getComponent<GraphicsComponent>().GetFlip() );

        if( Input::gamepadButtonDown(Gamepad::BUTTON_X, myGamepadId) )
        {
            ChangeShape(CONE_SHAPE);
        }
        else if( Input::gamepadButtonDown(Gamepad::BUTTON_A, myGamepadId) )
        {
            ChangeShape(CIRCLE_SHAPE);
        }
        else if( Input::gamepadButtonDown(Gamepad::BUTTON_Y, myGamepadId) )
        {
            ChangeShape(LINE_SHAPE);
        }

        Sint16 axisLeftTrigger = Input::getGamepadAxis(Gamepad::AXIS_LEFT_TRIGGER, myGamepadId);
        Sint16 axisRightTrigger = Input::getGamepadAxis(Gamepad::AXIS_RIGHT_TRIGGER, myGamepadId);

        if(currentGenerator != circleGenerator)
        {
            if( axisLeftTrigger > TRIGGER_DEADZONE ) {
                ChangeParticleType( PT_BLUE );
            }
            else if( axisRightTrigger > TRIGGER_DEADZONE ) {
                ChangeParticleType( PT_ORANGE );
            }
        }

        particleSystems[(int)currentType]->SetPos(entity->getComponent<TransformComponent>().position);

        if( std::max(axisLeftTrigger,axisRightTrigger) > TRIGGER_DEADZONE || Input::getKey(SDLK_q) )
        {
            // if( !entity->hasComponent<PlayerMana>() )
            //     return;

            if(entity->getComponent<PlayerMana>().GetMana() < manaCost)
            {
                entity->getComponent<PlayerMana>().UseMana(manaCost * 0.5f);
                return;
            }

            entity->getComponent<PlayerMana>().UseMana(manaCost);

            Emmit(emmitRate);
        }
    }
};