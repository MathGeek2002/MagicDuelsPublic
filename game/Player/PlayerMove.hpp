#ifndef PLAYER_MOVE
#define PLAYER_MOVE

#include "../../core/ECS/ECS.hpp"
#include "../../core/TransformComponent/TransformComponent.h"
#include "../../core/Input/Input.h"
#include <SDL2/SDL.h>
#include "../../thirdparty/glm/glm.hpp"
#include "../../core/Physics/Rigidbody.h"
#include "../../core/graphics/GraphicsComponent.h"
#include "../../core/Input/Gamepad.h"

class PlayerMove : public Component
{
private:
    glm::vec3 vel;
    bool isGrounded = true;
    bool isJumping = false;
    float acceleration = 5.f;
    Uint8 myGamepadId;

    void Grounded(Entity* collider)
    {
        if( collider->tag == "Ground" && entity->hasComponent<Rigidbody>()){

            float velocity_Y = entity->getComponent<Rigidbody>().velocity.y;
            velocity_Y = std::max(velocity_Y, 0.f);
            entity->getComponent<Rigidbody>().velocity.y = velocity_Y;
            isGrounded = true;
            entity->getComponent<Rigidbody>().useGravity = false;
            isJumping = false;
        }
        else if( !this->enable && entity->hasComponent<Rigidbody>() )
        {
            entity->getComponent<Rigidbody>().useGravity = true;
        }
    }
    
public:
    PlayerMove(Uint8 gamepadId_ ,glm::vec3 vel_ = glm::vec3(1,1,0)) : Component(CT_CUSTOM), vel(vel_), myGamepadId(gamepadId_)  {
        isGrounded = false;
    }

    void init() override
    {
        this->entity->onTriggerEnter.connect_member(this,&PlayerMove::Grounded);
    }

    void run(float dt) override
    {
        bool jumpInput = Input::gamepadButtonDown(Gamepad::BUTTON_B, myGamepadId) || (Input::keyDown(SDLK_w) && myGamepadId == 2 );

        if( jumpInput && isGrounded)
            Jump();

        CheckIfGrounded();

        Movement(dt);
    }

    void Jump()
    {
        if(entity->hasComponent<Rigidbody>())
        {
            entity->getComponent<Rigidbody>().AddForce( glm::vec3(0.f,7.f,0.f) );
            entity->getComponent<Rigidbody>().useGravity = true;
            isGrounded = false;
            isJumping = true;
        }
    }

    void CheckIfGrounded() {

        if(!isGrounded)
        {
            entity->getComponent<Rigidbody>().useGravity = true;
            //isJumping = true;
        }
        else
            isGrounded = false;
    }

    void Movement(float dt) 
    {
        float inputX = (float)Input::getGamepadAxis( Gamepad::AXIS_LEFT_X , myGamepadId) / INT16_MAX;

        if( myGamepadId == 2 )
            inputX += -(int)Input::getKey(SDLK_a) + (int)Input::getKey(SDLK_d);

        entity->getComponent<Rigidbody>().velocity.x = inputX * vel.x;

        float velocityX = entity->getComponent<Rigidbody>().velocity.x;

        entity->getComponent<Rigidbody>().velocity.x = glm::clamp(velocityX, -vel.x, vel.x );
    
        if( inputX < 0 && !entity->getComponent<GraphicsComponent>().GetFlip() )
            entity->getComponent<GraphicsComponent>().SetFlip(true);
        else if( inputX > 0 && entity->getComponent<GraphicsComponent>().GetFlip() )
            entity->getComponent<GraphicsComponent>().SetFlip(false);
    }

    inline bool IsGrounded() const {   return isGrounded;  }
    inline bool IsJumping() const  {   return isJumping;   }
};
#endif