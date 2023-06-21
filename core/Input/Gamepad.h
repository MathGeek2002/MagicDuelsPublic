#ifndef GAMEPAD_H
#define GAMEPAD_H
#define BUTTON_BIT(X) (1 << (X))

#include <SDL2/SDL.h>
#include "../../thirdparty/glm/glm.hpp"

class Gamepad
{
public:
    enum Axis
    {
        AXIS_LEFT_X = 0,
        AXIS_LEFT_Y,
        AXIS_RIGHT_Y,
        AXIS_RIGHT_X,
        AXIS_LEFT_TRIGGER,
        AXIS_RIGHT_TRIGGER,
        AXIS_MAX
    };
    enum Button
    {
        BUTTON_A = 0,
        BUTTON_B,
        BUTTON_X,
        BUTTON_Y,
        BUTTON_BACK,
        BUTTON_GUIDE,
        BUTTON_START,
        BUTTON_LEFTSTICK,
        BUTTON_RIGHTSTICK,
        BUTTON_LEFTSHOULDER,
        BUTTON_RIGHTSHOULDER,
        BUTTON_DPAD_UP,
        BUTTON_DPAD_DOWN,
        BUTTON_DPAD_LEFT,
        BUTTON_DPAD_RIGHT,
        BUTTON_MAX
    };

    enum GamepadNumber
    {
        GAMEPAD_1 = 1,
        GAMEPAD_2,
        GAMEPAD_3,
        GAMEPAD_4
    };

public:
    SDL_GameController *controllerRef;
    SDL_JoystickID joyId;

private:
    Uint32 buttonStates;
    Uint32 lastButtonStates;
    Sint16 analogStates[6];

public:
    Gamepad(SDL_GameController *);
    void postUpdate();
    void setButton(Uint8, bool);
    bool getButton(Button);
    bool buttonDown(Button);
    bool buttonUp(Button);
    void setAxis(Uint8, Sint16);
    Sint16 getAxis(Axis);
};

#endif