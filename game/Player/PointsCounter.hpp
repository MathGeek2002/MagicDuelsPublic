#pragma once

#include "../../core/UI/UI.h"
#include "SummaryScreen.hpp"

class PointsCounter
{
private:
    TextLabel *scoreLabels [2];
    int scores [2];
    Font *testfont;
    SummaryScreen * summaryScreen = nullptr;

    const int SCORES_TO_WIN = 3;

public:
    PointsCounter()
    {
        testfont = new Font("ASSETS/fonts/alagard.ttf", 16);

        scores[0] = 0;
        scoreLabels[0] = new TextLabel(20 , 50, testfont);
        scoreLabels[0]->SetTextScale(4.f);
        scoreLabels[0]->SetText("0");
        
        scores[1] = 0;
        scoreLabels[1] = new TextLabel(-40 , 50, testfont);
        scoreLabels[1]->SetAnchor(AnchorType::RIGHT_TOP);
        scoreLabels[1]->SetTextScale(4.f);
        scoreLabels[1]->SetText("0");
    }

    ~PointsCounter()
    {
        delete testfont;

        if(summaryScreen != nullptr)
            delete summaryScreen;
    }

    void AddPoint(int playerId)
    {
        scores[ playerId % 2 ]++;
        scoreLabels[ playerId % 2 ] -> SetText( std::to_string(scores[playerId % 2]).c_str() );

        if(scores[ playerId % 2 ] == SCORES_TO_WIN)
        {
            Application::gameFinished = true;
            summaryScreen = new SummaryScreen(playerId % 2 + 1);
        }
    }

    void Reset()
    {
        for(int i = 0;i < 2; i++) {
            scores[i] = 0;
            scoreLabels[i]->SetText("0");
        }
    }
};
