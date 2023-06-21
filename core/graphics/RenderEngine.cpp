#include "RenderEngine.h"
#include "../Application.h"
#include "Shader.h"
#include "../../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../../thirdparty/glm/gtc/type_ptr.hpp"
#include "../../thirdparty/glm/gtc/type_ptr.hpp"
#include "Sprite.h"
#include "Materials.h"
#include <set>
#include "../ECS/ECS.hpp"
#include "GraphicsComponent.h"
#include "../Input/Input.h"
#include "TextureSprite.h"
#include "../TransformComponent/TransformComponent.h"
#include "../UI/UI.h"

SDL_Window *RenderEngine::gameWindow = NULL;

std::string windowTitle = "Magic Dueals";
unsigned int maGL = 3; // major openGL version
unsigned int miGL = 3; // minor openGL version
unsigned int RenderEngine::screenWidth = 1440;
unsigned int RenderEngine::screenHeight = 900;

SDL_GLContext RenderEngine::glContext = NULL;

View *RenderEngine::view = nullptr;

Shader *shader;
Shader *textureShader;
Shader *fontShader;
Shader *gizmoShader;
Shader *particleShader;
Shader *uiShader;
Font *font;

float myTime;

bool spritesCmp(const GraphicsComponent *obj1, const GraphicsComponent *obj2)
{
    return (obj1->orderInLayer == obj2->orderInLayer) ? obj1 < obj2 : obj1->orderInLayer < obj2->orderInLayer;
}

std::set<GraphicsComponent *, decltype(&spritesCmp)> sprites(spritesCmp);
std::set<Collider *> colliders;
std::set<Gizmo *> models;
std::set<ParticleSystem *> particleSystems;

bool RenderEngine::drawGizmo = false;

void RenderEngine::InitData()
{
    glClearColor(0.2f, 0.4f, 0.2f, 1.f);

    view = new View(screenWidth, screenHeight);
    view->camera->pos = glm::vec3(0.f, 0.f, 2.f);
    view->camera->Update();

    shader = new Shader("Shaders/VertexShader.vert", "Shaders/TextureSprite.frag");
    gizmoShader = new Shader("Shaders/GizmoShader.vert", "Shaders/GizmoShader.frag");
    particleShader = new Shader("Shaders/ParticleShader.vert", "Shaders/ParticleShader.frag");
    uiShader = new Shader("Shaders/VertexShader.vert", "Shaders/TextureSprite.frag");

    font = new Font("ASSETS/fonts/Karla-Regular.ttf", 100);

    myTime = 0.f;
}

void RenderEngine::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->Use();

    view->SetView(shader);
    glUniform1f(glGetUniformLocation(shader->program, "uTime"), myTime);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto _sprite : sprites)
    {
        if( !_sprite -> enable ) 
            continue;

        _sprite->GetModel()->SetLocalTransform(_sprite->entity->getComponent<TransformComponent>().getModel());
        _sprite->GetModel()->Draw(shader);
    }

    glDisable(GL_BLEND);

    particleShader->Use();
    view->SetView(particleShader);

    for (auto particleSystem : particleSystems)
    {
        particleSystem->Draw(particleShader);
    }

    glm::mat4 projection = glm::ortho(
        0.0f, (float)screenWidth,
        0.0f, (float)screenHeight);

    if (gizmoShader != nullptr && drawGizmo)
    {
        gizmoShader->Use();
        view->SetView(gizmoShader);
        for (auto _collider : colliders)
            if(_collider->enable)
            {
                _collider->gizmo->SetLocalTransform(_collider->entity->getComponent<TransformComponent>().GetUnscaledModel());
                _collider->gizmo->Draw(gizmoShader);
            }

        for (auto _model : models)
            _model->Draw(gizmoShader);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // draw UI
    uiShader->Use();
    uiShader->SetMatrix4("view", glm::value_ptr(projection));

    for (UISprite *obj : UI::uiObjects)
    {
        obj->Draw(uiShader);
    }

    glDisable(GL_BLEND);
}

void RenderEngine::Update(double dt)
{
    // if (Input::getKey(SDLK_x))
    // {
    //     view->camera->pos += glm::vec3(0.f, 0.f, 2.f * dt);
    // }

    // if (Input::getKey(SDLK_z))
    // {
    //     view->camera->pos += glm::vec3(0.f, 0.f, -2.f * dt);
    // }

    myTime += dt;
}

void RenderEngine::UpdateView(SDL_Event &event)
{
    screenWidth = (unsigned int)event.window.data1;
    screenHeight = (unsigned int)event.window.data2;
    std::cout << "Window resize: " << screenWidth << " x "
              << screenHeight << std::endl;

    view->SetScreen(screenWidth, screenHeight);
}

bool RenderEngine::Init()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, maGL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, miGL);

    gameWindow = SDL_CreateWindow(windowTitle.c_str(),
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  screenWidth,
                                  screenHeight,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (gameWindow == NULL)
    {
        std::cout << "Window could not be created SDL Error : " << SDL_GetError() << std::endl;
        return false;
    }
    glContext = SDL_GL_CreateContext(gameWindow);

    if (glContext == NULL)
    {
        std::cout << "OpenGL context could not be created SDL Error : " << SDL_GetError() << std::endl;
        return false;
    }

    if (SDL_GL_SetSwapInterval(1) < 0)
    {
        std::cout << "Warrning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
    }

    if (!InitGL())
    {
        std::cout << "Unable to initialize OpenGL : " << std::endl;
        return false;
    }
    return true;
}

bool RenderEngine::InitGL()
{
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW " << std::endl;
        return false;
    }
    std::cout << "Open GL version:  " << glGetString(GL_VERSION) << std::endl;
    return true;
}

void RenderEngine::Close()
{
    delete shader;
    delete gizmoShader;
    delete uiShader;
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(gameWindow);
    gameWindow = NULL;

    sprites.clear();
}

void RenderEngine::RegisterObject(GraphicsComponent *graphicsComponent)
{
    sprites.insert(graphicsComponent);
}

void RenderEngine::UnRegisterObject(GraphicsComponent *graphicsComponent)
{
    sprites.erase(graphicsComponent);
}

void RenderEngine::RegisterGizmo(Collider *collider) { colliders.insert(collider); }
void RenderEngine::UnRegisterGizmo(Collider *collider) { colliders.erase(collider); }

void RenderEngine::RegisterModel(Gizmo *model) { models.insert(model); }

void RenderEngine::UnRegisterModel(Gizmo *model) { models.erase(model); }

void RenderEngine::RegisterParticleSystem(ParticleSystem *model) { particleSystems.insert(model); }
void RenderEngine::UnRegisterParticleSystem(ParticleSystem *model) { particleSystems.erase(model); }

void RenderEngine::UnregisterAllGraphicsComponents() 
{
    sprites.clear(); 
    UI::uiObjects.clear();
}