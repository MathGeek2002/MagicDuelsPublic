#ifndef MENU_SCENE_HPP
#define MENU_SCENE_HPP

#include "../../core/Scene/Scene.h"
#include "../../core/Application.h"
#include "../../core/UI/UI.h"
#include "../../core/Input/Input.h"
#include "../../core/Scene/SceneManager.h"
#include "../../core/Input/Gamepad.h"
#include "PlayerSelectionScene.hpp"
#include "../../core/Audio/Audio.h"
#include "../../core/Audio/Music.h"
#include "../../core/graphics/AnimatedSprite.h"
#include "../../core/graphics/GraphicsComponent.h"
#include "../../core/Audio/Audio.h"
#include "../../core/Audio/Sound.h"

class PlayerSelectionScene;

class MenuScene : public Scene
{
public:
    Font *uiFont;

    Music * mainTheme;

    AnimatedSprite * characterIcon = nullptr;

    Button * selectedButton = nullptr;
    TextLabel * selectedButtonsLabel = nullptr;

    Button *playButton;
    Button *exitButton;

    TextLabel *playLabel;
    TextLabel *exitLabel;

    Sound * clickSound;

    MenuScene() : Scene() {}

    void Init() override
    {
        CollisionManager::initialized = false;

        clickSound = new Sound("ASSETS/audio/click-button.mp3",100);
        uiFont = new Font("ASSETS/fonts/alagard.ttf", 16);

        TextLabel *titleLabel = new TextLabel(0, -200, uiFont);
        titleLabel->SetTextScale(6.f);
        titleLabel->SetText("MAGIC DUELS");
        titleLabel->SetAnchor(AnchorType::CENTER);
        

        playButton = new Button(0, -35, 300, 50);
        playButton->sprite->ChangeTexture("ASSETS/ui/button_blank.png");
        playButton->SetAnchor(AnchorType::CENTER);
        playButton->onClick.connect([]()
                                { SceneManager::LoadScene<PlayerSelectionScene>(); });


        playLabel = new TextLabel(0, -35, uiFont);
        playLabel->SetTextScale(3.f);
        playLabel->SetText("Play");
        playLabel->SetAnchor(AnchorType::CENTER);

        exitButton = new Button(0, 35, 300, 50);
        exitButton->sprite->ChangeTexture("ASSETS/ui/button_blank.png");
        exitButton->SetAnchor(AnchorType::CENTER);
        exitButton->onClick.connect([]()
                                 { Application::quit = true; });

        exitLabel = new TextLabel(0, 35, uiFont);
        exitLabel->SetTextScale(3.f);
        exitLabel->SetText("Exit");
        exitLabel->SetAnchor(AnchorType::CENTER);

        mainTheme = new Music("ASSETS/audio/LamentForWarriorsSoul.mp3",100);
        Audio::playMusic(mainTheme, true);
    }

    void Update(float dt) override
    {
        if(Input::gamepadButtonDown(Gamepad::BUTTON_DPAD_UP))
        {
            clickSound->play();
            SetSelectedButton(playButton, playLabel);
        }

        if(Input::gamepadButtonDown(Gamepad::BUTTON_DPAD_DOWN))
        {
            clickSound->play();
            SetSelectedButton(exitButton,exitLabel);
        }
        if(Input::gamepadButtonDown(Gamepad::BUTTON_A) && selectedButton != nullptr)
            selectedButton->onClick.emit();
    }

    void SetSelectedButton(Button * newSelectedButton, TextLabel * buttonsLabel)
    {
        if(selectedButton != nullptr)
            selectedButtonsLabel->SetColor(1.f,1.f,1.f);

        selectedButton = newSelectedButton;
        selectedButtonsLabel = buttonsLabel;
        selectedButtonsLabel->SetColor(1.f,1.f,0.f);
    }

    ~MenuScene()
    {
        delete uiFont;
        delete clickSound;

        if(characterIcon != nullptr)
            delete characterIcon;
    }
};

#endif