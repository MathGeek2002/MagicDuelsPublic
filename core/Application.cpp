#include "Application.h"
#include "graphics/RenderEngine.h"
#include "Input/Input.h"
#include "Audio/Audio.h"
#include "UI/UI.h"
#include "TransformComponent/TransformComponent.h"
#include "Scene/SceneManager.h"

double Application::fpsTimerSpan = 1.;

bool Application::quit = false;
bool lShift = false;
bool timePaused = false;
bool wireDraw = false;

Manager *objectManager;

Signal <> Application::onRestart;
Signal <> Application::onWindowResized;

bool Application::gameFinished = false;

bool Application::Init()
{
    objectManager = new Manager();

    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error: "
                  << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        success = RenderEngine::Init();
    }

    Input::Init();
    Audio::Init();
    Audio::setMasterVolume(64);
    UI::Init();

    return success;
}

void Application::Close()
{
    Application::onRestart.disconnect_all();
    Application::onWindowResized.disconnect_all();
  
    CollisionManager::Close();
    UI::Close();
    RenderEngine::Close();
    Audio::Close();
    Input::Destroy();
    objectManager->removeAll();

    SDL_Quit();
}

void Application::Update(double dt)
{
    bool sceneChanged = SceneManager::Update();
    // long scene loading times can cause unwanted updates with high dt value
    if (sceneChanged)
    {
        std::cout << "After scene change update: " << dt << std::endl; 
        return;
    }

    RenderEngine::Update(dt);
    objectManager->Update(dt);
    CollisionManager::Update();
    UI::Update(dt);
}

void Application::PollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        Input::ProcessEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                RenderEngine::UpdateView(event);
                Application::onWindowResized.emit();
                break;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                quit = true;
                break;

            case SDLK_LSHIFT:
                lShift = true;
                break;

            case SDLK_l:
                if (!wireDraw)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    wireDraw = true;
                    std::cout << "Wireframe" << std::endl;
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    wireDraw = false;
                    std::cout << "Solid" << std::endl;
                }
                break;

            case SDLK_g:
                RenderEngine::drawGizmo = !RenderEngine::drawGizmo;
                break;
            }
            break;
        }
    }
}

void Application::Run()
{

    unsigned long hiResFreq = SDL_GetPerformanceFrequency();
    double hiResTick = 1. / hiResFreq;
    unsigned long currentTime = SDL_GetPerformanceCounter();
    unsigned long lastTime = currentTime;
    std::cout << "Hi-resolution tick: " << hiResTick << " sec. " << std::endl;
    SDL_Delay(1);

    double fpsTimer = 0.;
    while (!quit)
    {
        currentTime = SDL_GetPerformanceCounter();
        unsigned long elapsedTime = currentTime - lastTime;
        lastTime = currentTime;
        double dt = elapsedTime * hiResTick;
        fpsTimer += dt;

        double fps = 1. / dt;
        if (fpsTimer > Application::fpsTimerSpan)
        {
            fpsTimer -= Application::fpsTimerSpan;
            std::cout << "FPS = " << fps << std::endl;
        }

        PollEvents();
        Input::Update();

        if (!timePaused)
        {
            if (SceneManager::currentScene != nullptr)
            {
                // call custom update func
                SceneManager::currentScene->Update(dt);
            }
            Application::Update((float)dt);
        }
        RenderEngine::Draw();

        SDL_GL_SwapWindow(RenderEngine::gameWindow);
        Input::postUpdate();
    }
}

Entity &
Application::Instantiate(const glm::vec3 position_)
{
    Entity &ent = objectManager->addEntity();
    SceneManager::addObjectToCurrentScene(&ent);
    ent.addComponent<TransformComponent>(position_);
    return ent;
}

void Application::Register(Entity *e)
{
    objectManager->addEntity(e);
    SceneManager::addObjectToCurrentScene(e);
}