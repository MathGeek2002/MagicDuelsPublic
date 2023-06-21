#pragma once

#include "../../core/UI/UI.h"
#include "../../core/UI/UISprite.h"
#include "../../core/UI/Button.h"
#include "../../core/Scene/Scene.h"

class SummaryScreen
{
private:
    UISprite * background;
    Button * buttonMenu;
    Button * playAgainButton;
    Font *textFont;
    TextLabel * whoWinLabel;
    TextLabel * menuText;
    TextLabel * playAgainText;

public:
    SummaryScreen(int playerId)
    {
        textFont = new Font("ASSETS/fonts/alagard.ttf", 16);
        background = new UISprite(0,0,400,400,"ASSETS/Sprites/SummaryScreen.png");
        background->SetAnchor(AnchorType::CENTER);

        buttonMenu = new Button(-90, 80, 120, 40);
        buttonMenu->sprite->ChangeTexture("ASSETS/ui/button_blank.png");
        buttonMenu->SetAnchor(AnchorType::CENTER);
        buttonMenu->onClick.connect([]()
                                { 
                                std::cout << "menu button clicked\n";
                                SceneManager::LoadScene<MenuScene>(); });

        menuText = new TextLabel(-90,80,textFont);
        menuText->SetTextScale(2.f);
        menuText->SetText("MENU");
        menuText->SetAnchor(AnchorType::CENTER);

        playAgainButton = new Button(90, 80, 120, 40);
        playAgainButton->sprite->ChangeTexture("ASSETS/ui/button_blank.png");
        playAgainButton->SetAnchor(AnchorType::CENTER);
        playAgainButton->onClick.connect([]()
                                { 
                                std::cout << "restart button clicked\n";
                                SceneManager::LoadScene<MainScene>(); });
        
        playAgainText = new TextLabel(90,80, textFont);
        playAgainText->SetTextScale(1.3f);
        playAgainText->SetText("PLAY AGAIN");
        playAgainText->SetAnchor(AnchorType::CENTER);

        whoWinLabel = new TextLabel(0, -100, textFont);
        whoWinLabel->SetTextScale(3.5f);
        whoWinLabel->SetText( ("Win player " + std::to_string(playerId)).c_str());
        whoWinLabel->SetAnchor(AnchorType::CENTER);
    }

    ~SummaryScreen()
    {
        delete textFont;
    }
};