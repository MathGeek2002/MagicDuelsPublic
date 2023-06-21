#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ctime>
#include <iostream>
#include "../../thirdparty/glm/glm.hpp"

class Random
{
private:
    static unsigned long int seed;

public:
    static void Init()
    {
        seed = (unsigned)time(NULL);
        std::cout << "Seed: " << seed << std::endl;
    }

    static void Init(unsigned int seed_)
    {
        seed = seed_;
        std::cout << "Seed: " << seed << std::endl;
    }

    static int rand()
    {
        seed = 1103515245 * seed + 12345;
        return (unsigned int)(seed/65536) % 32768;
    }

    static int Range(int a, int b)
    {
        return rand() % (b - a + 1) + a;
    }

    static float Range(float a, float b)
    {
        return ((float)rand() / 32767) * (b - a) + a;
    }

    static glm::vec2 RandDirection(float minAngle,float maxAngle)
    {
        float angle = glm::radians( Range(minAngle, maxAngle) );

        return glm::vec2( glm::cos(angle), glm::sin(angle) );
    }
};

#endif