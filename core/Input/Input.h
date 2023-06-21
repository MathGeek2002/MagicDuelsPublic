#ifndef INPUT_H
#define INPUT_H
#define MAX_GAMEPADS_NUM 4

#include <SDL2/SDL.h>
#include "../../thirdparty/glm/glm.hpp"
#include "Gamepad.h"

class Input
{
public:
    enum MouseButton
    {
        MB_LEFT = SDL_BUTTON_LEFT,
        MB_RIGHT = SDL_BUTTON_RIGHT,
        MB_MIDDLE = SDL_BUTTON_MIDDLE,
        MB_BUTTON4 = SDL_BUTTON_X1,
        MB_BUTTON5 = SDL_BUTTON_X2
    };

private:
    static const unsigned char *currKeyStates;
    static unsigned char *lastKeyStates;
    static int numkeys;
    static Uint32 currMouseState;
    static Uint32 lastMouseState;
    static int mouseX, mouseY;
    static Gamepad *gamepads[MAX_GAMEPADS_NUM];

public:
    static void Init();
    static void Update();
    static void postUpdate();
    static void ProcessEvent(SDL_Event *);
    static bool getKey(SDL_Keycode);
    static bool keyDown(SDL_Keycode);
    static bool keyUp(SDL_Keycode);
    static bool getMouseButton(MouseButton);
    static bool mouseDown(MouseButton);
    static bool mouseUp(MouseButton);
    static glm::vec2 getMousePosition();
    static bool getGamepadButton(Gamepad::Button, Uint8);
    static Sint16 getGamepadAxis(Gamepad::Axis, Uint8);
    static Uint8 gamepadButtonDown(Gamepad::Button, Uint8 = 0);
    static Uint8 gamepadButtonUp(Gamepad::Button, Uint8 = 0);
    static void Destroy();

private:
    static void processGamepadButtonEvent(SDL_ControllerButtonEvent);
    static void processGamepadAxisEvent(SDL_ControllerAxisEvent);
    static bool addGamepad(SDL_JoystickID);
    static bool removeGamepad(SDL_JoystickID);
};

#endif