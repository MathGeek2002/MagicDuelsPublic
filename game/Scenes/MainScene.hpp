#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#include "../../core/Scene/Scene.h"
#include "../world/world.hpp"
#include "../../core/Application.h"
#include "../../core/UI/UI.h"
#include "../../core/Input/Input.h"
#include "../../core/graphics/RenderEngine.h"
#include "MenuScene.hpp"
#include "../../core/graphics/AnimatedSprite.h"
#include "../Player/PlayerMove.hpp"
#include "../../core/StateMachine/PlayerAnimator.hpp"
#include "../../core/ParticleSystem/ParticlesManager.hpp"
#include "../Player/PlayerHealth.hpp"
#include "../Player/PlayerRestartMananger.hpp"
#include "../Player/PointsCounter.hpp"
#include "../../core/UI/UISprite.h"
#include "../world/PotionController.hpp"
#include <fstream>
#include "../../core/graphics/Sprite.h"
#include "../../core/Audio/Audio.h"
#include "../../core/Audio/Music.h"
#include "../../core/Input/Gamepad.h"
//#include "../../core/Debug/Debug.hpp"

class MenuScene;

class MainScene : public Scene
{
public:
    World *world;
    Font *uiFont;
    Entity *player1;
    Entity *testEntity;

    PointsCounter *pointCounter;
    glm::vec3 spawnPos = glm::vec3(-0.8f, .1f, 0);
    glm::vec3 spawnPos2 = glm::vec3(1.f, 0.f, 0.f);

    UISprite *iconX;
    UISprite *iconCone;

    UISprite *iconY;
    UISprite *iconLine;

    UISprite *iconA;
    UISprite *iconSphere;

    UISprite * iconRT;
    UISprite * iconLT;

    UISprite * elementsIcon;
    Button *buttonMenu;

    TextLabel * menuButtonLabel;

    Uint8 firstGamepad;
    Uint8 secondGamepad;

    Sprite* backgroundSprite;

    std::ifstream gamepadDataFile;

    Music * mainTheme;


    MainScene() : Scene() {}

    void Init() override
    {
        Application::gameFinished = false;
        Application::onRestart.disconnect_all();

        world = new World();

        pointCounter = new PointsCounter();

        uiFont = new Font("ASSETS/fonts/alagard.ttf", 16);

        LoadGamepadData();

        CreateBackGround();

        CreatePlayer(spawnPos);
        CreatePlayer2(spawnPos2);

        buttonMenu = new Button(5, 5, 120, 40);
        buttonMenu->sprite->ChangeTexture("ASSETS/ui/button_blank.png");
        buttonMenu->onClick.connect([]()
                                { 
                                std::cout << "menu button clicked\n";
                                SceneManager::LoadScene<MenuScene>(); });


        menuButtonLabel = new TextLabel(20, 15, uiFont);
        menuButtonLabel->SetTextScale(1.9f);
        menuButtonLabel->SetText("MENU");
                

        CreateUI();
        CreateManaPotion();
        CreateHealthPotion();
        CollisionManager::Init(world->worldWidth, world->worldHeight);

        mainTheme = new Music("ASSETS/audio/To Arms!.mp3",100);
        Audio::playMusic(mainTheme,true);
    }

    void Update(float dt) override
    {
        if(Input::gamepadButtonDown(Gamepad::BUTTON_BACK))
            SceneManager::LoadScene<MenuScene>();
    }

    ~MainScene()
    {
        // objects should be deleted by scene object
        delete uiFont;

        delete pointCounter;
        delete backgroundSprite;
    }

    void CreatePlayer(glm::vec3 pos)
    {
        Entity &character = Application::Instantiate(pos);
        // AnimatedSprite *sprite = new AnimatedSprite(8, 100, "ASSETS/Wizard1/Idle.png");
        AnimatedSprite *sprite = new AnimatedSprite(.7f, .7f);
        sprite->addAnimation("idle", "ASSETS/Wizard1/Idle.png", 8, 100);
        sprite->addAnimation("attack", "ASSETS/Wizard1/Attack1.png", 8, 100);
        sprite->addAnimation("run", "ASSETS/Wizard1/Run.png", 8, 100);
        sprite->addAnimation("jump", "ASSETS/Wizard1/Jump.png", 2, 100);
        sprite->addAnimation("fall", "ASSETS/Wizard1/Fall.png", 2, 100);
        sprite->addAnimation("death", "ASSETS/Wizard1/Death.png", 7, 100, false);

        // sprite->setOffset(0.f, -0.01f);
        // sprite->setScale(1, 1);

        character.addComponent<GraphicsComponent>();
        character.getComponent<TransformComponent>().SetAngle(0);
        character.getComponent<TransformComponent>().UpdateOrientation();
        character.getComponent<TransformComponent>().UpdateModel();
        character.getComponent<GraphicsComponent>().SetModel(sprite);

        // character.addComponent<ColliderAABB>(0.15f, 0.32f);
        // character.addComponent<SphereCollider>(0.2f);
        character.addComponent<CapsuleCollider>(glm::vec3(0, -0.04f, 0), 0.03f, 0.15f);

        Vertex *vertices = new Vertex[4];

        vertices[0].position = glm::vec3(0.021f, -0.1f, 0);
        vertices[1].position = glm::vec3(-0.021f, -0.1f, 0);
        vertices[2].position = glm::vec3(-0.021f, -0.12f, 0);
        vertices[3].position = glm::vec3(0.021f, -0.12f, 0);

        // character.addComponent<MeshCollider>(vertices,4u);

        character.tag = "Player";
        character.addComponent<PlayerMove>(firstGamepad, glm::vec3(1.f, 1.f, 0));
        character.addComponent<PlayerAnimator>();
        character.addComponent<MeshCollider>(vertices, 4);
        character.getComponent<MeshCollider>().isTrigger = true;
        // character.addComponent<ColliderAABB>(0.05f,0.02f,0.f, glm::vec3(0.f,-0.11f,0.f));
        // character.getComponent<ColliderAABB>().SetTriggerMode(true);
        character.addComponent<ParticleManager>(firstGamepad);
        character.getComponent<ParticleManager>().ChangeShape(LINE_SHAPE);
        character.addComponent<Rigidbody>(2.f, 0.5f);

        character.addComponent<PlayerHealth>(1000.f, 1);
        character.getComponent<PlayerHealth>().onDeath.connect_member(pointCounter, &PointsCounter::AddPoint);
        character.addComponent<PlayerMana>(2000.f, 100.f);
        character.addComponent<PlayerRestartManager>(pos);
    }

    void CreatePlayer2(glm::vec3 pos)
    {
        Entity &character = Application::Instantiate(pos);
        AnimatedSprite *animatedSprite = new AnimatedSprite(0.7f, 0.7f);
        animatedSprite->addAnimation("idle", "ASSETS/Wizard4/Idle.png", 8, 100);
        animatedSprite->addAnimation("attack", "ASSETS/Wizard4/Attack1.png", 8, 100);
        animatedSprite->addAnimation("run", "ASSETS/Wizard4/Run.png", 8, 100);
        animatedSprite->addAnimation("jump", "ASSETS/Wizard4/Jump.png", 2, 100);
        animatedSprite->addAnimation("fall", "ASSETS/Wizard4/Fall.png", 2, 100);
        animatedSprite->addAnimation("death", "ASSETS/Wizard4/Death.png", 7, 100, false);

        //animatedSprite->setColor(0.9f, 0.7f, 0.5f);

        character.addComponent<GraphicsComponent>();
        character.getComponent<TransformComponent>().SetAngle(0);
        character.tag = "Player";

        Vertex *vertices = new Vertex[4];

        vertices[0].position = glm::vec3(0.021f, -0.1f, 0);
        vertices[1].position = glm::vec3(-0.021f, -0.1f, 0);
        vertices[2].position = glm::vec3(-0.021f, -0.12f, 0);
        vertices[3].position = glm::vec3(0.021f, -0.12f, 0);

        character.addComponent<MeshCollider>(vertices, 4);
        character.getComponent<MeshCollider>().isTrigger = true;

        character.getComponent<TransformComponent>().UpdateOrientation();
        character.getComponent<TransformComponent>().UpdateModel();
        character.getComponent<GraphicsComponent>().SetModel(animatedSprite);

        animatedSprite->setFlipH(true);

        character.addComponent<CapsuleCollider>(glm::vec3(0, -0.04f, 0), 0.03f, 0.15f);

        character.addComponent<PlayerMove>(secondGamepad, glm::vec3(1.f, 1.f, 0));
        character.addComponent<PlayerAnimator>();

        character.addComponent<ParticleManager>(secondGamepad);
        character.getComponent<ParticleManager>().ChangeParticleType(PT_BLUE);
        character.getComponent<ParticleManager>().ChangeShape(LINE_SHAPE);

        character.addComponent<PlayerHealth>(1000.f, 2);
        character.getComponent<PlayerHealth>().onDeath.connect_member(pointCounter, &PointsCounter::AddPoint);
        character.addComponent<PlayerMana>(2000.f, 100.f);

        character.addComponent<Rigidbody>(2.f, 0.5f);
        character.addComponent<PlayerRestartManager>(pos);
    }

    void CreateUI()
    {
        int x = -170;
        int y = -145;

        int y2 = y + 55;

        iconCone = new UISprite( x, y,      64,64,"ASSETS/Sprites/cone.png");
        iconCone->SetAnchor(AnchorType::BOTTOM);
        iconX = new UISprite( x + 15, y2,    32,32,"ASSETS/Sprites/button_X.png");
        iconX->SetAnchor(AnchorType::BOTTOM);

        iconLine = new UISprite( 75 + x, y,     64,64,"ASSETS/Sprites/line.png");
        iconLine->SetAnchor(AnchorType::BOTTOM);
        iconY = new UISprite( 90 + x, y2,   32,32,"ASSETS/Sprites/button_Y.png");
        iconY->SetAnchor(AnchorType::BOTTOM);

        iconSphere = new UISprite( 150 + x, y,   64,64,"ASSETS/Sprites/shield.png");
        iconSphere->SetAnchor(AnchorType::BOTTOM);
        iconA = new UISprite( 165 + x, y2,   32,32,"ASSETS/Sprites/button_A.png");
        iconA->SetAnchor(AnchorType::BOTTOM);

        elementsIcon = new UISprite( 225 + x, y, 64,64,"ASSETS/Sprites/elements.png");
        elementsIcon->SetAnchor(AnchorType::BOTTOM);
        iconLT = new UISprite( 225 + x, y2,  32,32,"ASSETS/Sprites/button_LT.png");
        iconLT->SetAnchor(AnchorType::BOTTOM);
        iconRT = new UISprite( 260 + x, y2,  32,32,"ASSETS/Sprites/button_RT.png");
        iconRT->SetAnchor(AnchorType::BOTTOM);
    }

    void CreateManaPotion()
    {
        Entity& potion = Application::Instantiate(glm::vec3(0.f,0.3f,0.f));
        AnimatedSprite *animatedSprite = new AnimatedSprite(0.05f, 0.05f);
        animatedSprite->addAnimation("potionIdle", "ASSETS/Sprites/blue_potion.png", 8, 100);
        potion.addComponent<GraphicsComponent>();
        potion.getComponent<GraphicsComponent>().SetModel(animatedSprite);

        potion.addComponent<SphereCollider>(0.025f);
        potion.addComponent<ManaPotion>(5.f,700.f);
        potion.addComponent<Rigidbody>(1.f, 0.f);
    }

    void CreateHealthPotion()
    {
        Entity& potion = Application::Instantiate(glm::vec3(0.2f,0.4f,0.f));
        AnimatedSprite *animatedSprite = new AnimatedSprite(0.05f, 0.05f);
        animatedSprite->addAnimation("potionIdle", "ASSETS/Sprites/red_potion.png", 8, 100);
        potion.addComponent<GraphicsComponent>();
        potion.getComponent<GraphicsComponent>().SetModel(animatedSprite);

        potion.addComponent<SphereCollider>(0.025f);
        potion.addComponent<HealthPotion>(5.f,200.f);
        potion.addComponent<Rigidbody>(1.f, 0.f);
    }

    void LoadGamepadData()
    {
        gamepadDataFile.open("gamepads_data.dat", std::ios::in | std::ios::binary);

        if(gamepadDataFile.is_open())
        {
            gamepadDataFile.read((char*)&firstGamepad, sizeof(Uint8));
            gamepadDataFile.read((char*)&secondGamepad, sizeof(Uint8));
        
            gamepadDataFile.close();
        }
        else
        {
            firstGamepad = (Uint8)1;
            secondGamepad = (Uint8)2;
        }
    }

    void CreateBackGround()
    {
        Entity& background = Application::Instantiate(glm::vec3(0,0,0));
        backgroundSprite = new Sprite(3,3,"ASSETS/Sprites/forest.png");
        backgroundSprite->setColor(0.5f,0.5f,0.5f);

        background.addComponent<GraphicsComponent>();
        background.getComponent<GraphicsComponent>().SetModel(backgroundSprite);
        background.getComponent<GraphicsComponent>().orderInLayer = -1;

    }
};

#endif