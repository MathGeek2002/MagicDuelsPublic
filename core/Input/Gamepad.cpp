#include "Gamepad.h"
#include <iostream>

#define DEAD_ZONE 6000

Gamepad::Gamepad(SDL_GameController *sdlControllerRef)
{
    controllerRef = sdlControllerRef;
    Gamepad::buttonStates = 0;
    for (int i = 0; i < Axis::AXIS_MAX; i++)
    {
        Gamepad::analogStates[i] = 0;
    }
    SDL_Joystick *joy = SDL_GameControllerGetJoystick(sdlControllerRef);
    joyId = SDL_JoystickInstanceID(joy);
}

void Gamepad::postUpdate()
{
    lastButtonStates = buttonStates;
}

void Gamepad::setButton(Uint8 button, bool state)
{
    if (state)
    {
        buttonStates |= (1 << button);
    }
    else
    {
        buttonStates &= ~(1 << button);
    }
}

bool Gamepad::getButton(Gamepad::Button button)
{
    return buttonStates & BUTTON_BIT(button);
}

bool Gamepad::buttonDown(Gamepad::Button button)
{
    return (buttonStates & BUTTON_BIT(button)) && !(lastButtonStates & BUTTON_BIT(button));
}

bool Gamepad::buttonUp(Gamepad::Button button)
{
    return !(buttonStates & BUTTON_BIT(button)) && (lastButtonStates & BUTTON_BIT(button));
}

void Gamepad::setAxis(Uint8 axis, Sint16 value)
{
    analogStates[axis] = value;
}

Sint16 Gamepad::getAxis(Gamepad::Axis axis)
{
    if (axis < Axis::AXIS_MAX)
    {
        return std::abs(analogStates[axis]) > DEAD_ZONE ? analogStates[axis] : 0.f;
    }
    return 0;
}