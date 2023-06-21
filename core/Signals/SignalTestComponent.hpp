#ifndef SIGNAL_TEST_COMPONENT
#define SIGNAL_TEST_COMPONENT

#include "../ECS/ECS.hpp"
#include <iostream>
#include <string>
#include "../../thirdparty/glm/glm.hpp"

class SignalTest : public Component
{
public:
    void PrintMessege(Entity * collider) const
    {
        if(collider->tag == "Player")
            std::cout<<"Aaa nie tak mocno!\n";
    }

    SignalTest() : Component(CT_CUSTOM) { }

    ~SignalTest() {}

    void init() override
    {
        this->entity->onCollisionEnter.connect_member(this,&SignalTest::PrintMessege);
    }

};

#endif