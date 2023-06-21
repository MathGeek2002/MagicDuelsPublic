#ifndef APPLICATION_H
#define APPLICATION_H

#include "ECS/ECS.hpp"
#include "Collisions/CollisionManager.h"
#include "Signals/Signal.hpp"
#include "Scene/Scene.h"

class Application
{
public:
    static bool quit;
    static bool gameFinished;

    static bool Init();
    static void Run();
    static void Close();
    static void PollEvents();
    static void Update(double delta);
    static double fpsTimerSpan;
    static Entity &Instantiate(const glm::vec3 position_);
    static void Register(Entity *e);
    static CollisionManager *collisionManager;
    static Signal <> onRestart;
    static Signal <> onWindowResized;
};

#endif
