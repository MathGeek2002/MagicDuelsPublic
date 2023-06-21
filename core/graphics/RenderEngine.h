#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#ifdef _WIN32

#define SDL_MAIN_HANDLED

#endif

#include<GL/glew.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
#include<GL/glu.h>
#include<iostream>
#include<string>
#include "GraphicsComponent.h"
#include "../Collisions/Collider.h"
#include "Gizmo.h"
#include "../ParticleSystem/ParticleSystem.h"
#include "View.h"

class RenderEngine{
  public:

    static unsigned int screenWidth, screenHeight;
    static bool drawGizmo;
    static View *view;
    static SDL_Window* gameWindow;
    static SDL_GLContext glContext;

    static bool Init();
    static bool InitGL();
    static void UpdateView(SDL_Event &);
    static void InitData();
    static void Update(double);
    static void Draw();
    static void Close();
    static void RegisterObject( GraphicsComponent* );
    static void UnRegisterObject( GraphicsComponent* );

    static SDL_Window* GetGameWindow() { return gameWindow; }
    static SDL_GLContext GetContextOGL33() { return glContext; }

    static void RegisterGizmo( Collider* );
    static void UnRegisterGizmo( Collider* );
    static void RegisterModel( Gizmo* model );
    static void UnRegisterModel( Gizmo* model );
    static void RegisterParticleSystem( ParticleSystem* particleSystem );
    static void UnRegisterParticleSystem( ParticleSystem* ParticleSystem );
    static void UnregisterAllGraphicsComponents();
};

#endif
