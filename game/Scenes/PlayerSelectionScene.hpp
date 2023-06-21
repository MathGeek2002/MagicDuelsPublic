#ifndef TEST_SCENE_HPP
#define TEST_SCENE_HPP

#include "../../core/Scene/Scene.h"
#include "../../core/Application.h"
#include "../../core/UI/UI.h"
#include "../../core/Input/Input.h"
#include "../../core/Input/Gamepad.h"
#include "../../core/Scene/SceneManager.h"
#include "../../core/graphics/RenderEngine.h"
#include "MenuScene.hpp"
#include "MainScene.hpp"
#include <fstream>
#include "../../core/graphics/AnimatedSprite.h"
#include "../../core/graphics/GraphicsComponent.h"
#include "../../core/graphics/RenderEngine.h"

// forward definition (for circular dependecies)
class MenuScene; 
class MainScene;

class PlayerSelectionScene : public Scene
{
private:
    Font *uiFont;

    Uint8 firstGamepadNumber = 0;
    Uint8 secondGamepadNumber = 0;

    TextLabel *testLabel;
    TextLabel * fisrtStatusLabel;
    TextLabel * secondStatusLabel;

    UISprite * iconA;

    UISprite * iconB;

    std::ofstream binaryFile;

    AnimatedSprite * characterIcon = nullptr;
    AnimatedSprite * characterIcon2 = nullptr;

public:
    PlayerSelectionScene() : Scene() {}

    void Init() override
    {
        uiFont = new Font("ASSETS/fonts/alagard.ttf", 16);
        TextLabel *testLabel = new TextLabel(5, 50, uiFont);
        testLabel->SetTextScale(4.f);
        testLabel->SetText("Players selection scene");

        // Button *button = new Button(RenderEngine::screenWidth / 2 - 150, 350, 300, 50);
        // button->sprite->ChangeTexture("ASSETS/ui/button_blank.png");
        // button->onClick.connect([]()
        //                         { SceneManager::LoadScene<MainScene>(); });

        fisrtStatusLabel = new TextLabel(20, -80, uiFont);
        fisrtStatusLabel->SetTextScale(3.f);
        fisrtStatusLabel->SetText("Not selected");
        fisrtStatusLabel->SetAnchor(AnchorType::CENTER);

        iconA = new UISprite(-160, -80, 50,50, "ASSETS/Sprites/button_A.png");
        iconA->SetAnchor(AnchorType::CENTER);

        CreatePlayerAvatar(glm::vec3(0.45f,0.2f,0.f), characterIcon, "ASSETS/Wizard1/Idle.png");

        secondStatusLabel = new TextLabel(20, 80, uiFont);
        secondStatusLabel->SetTextScale(3.f);
        secondStatusLabel->SetText("Not selected");
        secondStatusLabel->SetAnchor(AnchorType::CENTER);

        iconB = new UISprite(-160, 80, 50,50, "ASSETS/Sprites/button_B.png");
        iconB->SetAnchor(AnchorType::CENTER);
        CreatePlayerAvatar(glm::vec3(0.45f,-0.1f,0.f), characterIcon2, "ASSETS/Wizard4/Idle.png");

        binaryFile.open("gamepads_data.dat", std::ios::out | std::ios::binary);
    }

    void Update(float dt) override
    {
        if(firstGamepadNumber == 0)
        {
            for(Uint8 i = 1;i <= 4; i++)
                if(Input::gamepadButtonDown(Gamepad::BUTTON_A, i) && i != secondGamepadNumber)
                {
                    firstGamepadNumber = i;
                    fisrtStatusLabel->SetText("Selected");
                    break;
                }
            
            if( Input::getKey(SDLK_a) )
            {
                firstGamepadNumber = 1;
                fisrtStatusLabel->SetText("Selected");
            }

        }
        
        if(secondGamepadNumber == 0)
        {
            for(Uint8 i = 1;i <= 4; i++)
                if(Input::gamepadButtonDown(Gamepad::BUTTON_B, i) && i != firstGamepadNumber)
                {
                    secondGamepadNumber = i;
                    secondStatusLabel->SetText("Selected");
                    break;
                }
            
            if( Input::getKey(SDLK_b) )
            {
                secondGamepadNumber = 2;
                secondStatusLabel->SetText("Selected");
            }
        }
        
        if(firstGamepadNumber != 0 && secondGamepadNumber != 0)
        {
            binaryFile.write((char*)&firstGamepadNumber, sizeof(Uint8));
            binaryFile.write((char*)&secondGamepadNumber, sizeof(Uint8));
            SceneManager::LoadScene<MainScene>();
        }
    }

    ~PlayerSelectionScene()
    {
        delete uiFont;
        binaryFile.close();

        if(characterIcon != nullptr)
            delete characterIcon;

        if(characterIcon2 != nullptr)
            delete characterIcon2;
    }

    void CreatePlayerAvatar(glm::vec3 pos,AnimatedSprite * animatedSprite,const char * animationPath)
    {
        Entity& characterAvater = Application::Instantiate(pos);
        animatedSprite = new AnimatedSprite(.8f,.8f);
        animatedSprite->addAnimation("idle", animationPath, 8, 100);
        characterAvater.addComponent<GraphicsComponent>();
        characterAvater.getComponent<GraphicsComponent>().SetModel(animatedSprite);
    }
};

#endif
