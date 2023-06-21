#include "Input.h"
#include <iostream>

const unsigned char *Input::currKeyStates = nullptr;
unsigned char *Input::lastKeyStates = nullptr;
int Input::numkeys = 0;
Uint32 Input::currMouseState = 0;
Uint32 Input::lastMouseState = 0;
int Input::mouseX = 0;
int Input::mouseY = 0;
Gamepad *Input::gamepads[MAX_GAMEPADS_NUM] = {nullptr};

void Input::Init()
{
    currKeyStates = SDL_GetKeyboardState(&numkeys);
    lastKeyStates = new unsigned char[numkeys];

    currMouseState = SDL_GetMouseState(&mouseX, &mouseY);

    int joyDevices = SDL_NumJoysticks();
    for (int i = 0; i < joyDevices; i++)
    {
        if (SDL_IsGameController(i))
        {
            std::cout << "Gamepad detected at index: " << i << std::endl;
            addGamepad(i);
        }
    }

    std::cout << "Input initialized with size: " << numkeys << std::endl;
};

void Input::Update()
{
    currMouseState = SDL_GetMouseState(&mouseX, &mouseY);
}

void Input::postUpdate()
{
    for (int i = 0; i < numkeys; ++i)
        lastKeyStates[i] = currKeyStates[i];

    lastMouseState = currMouseState;
    for (int i = 0; i < MAX_GAMEPADS_NUM; i++)
    {
        if (gamepads[i] != nullptr)
        {
            gamepads[i]->postUpdate();
        }
    }
};

void Input::ProcessEvent(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
        processGamepadButtonEvent(event->cbutton);
        break;
    case SDL_CONTROLLERAXISMOTION:
        processGamepadAxisEvent(event->caxis);
        break;
    case SDL_CONTROLLERDEVICEREMOVED:
        std::cout << "Gamepad disconnected: " << event->cdevice.which << std::endl;
        removeGamepad(event->cdevice.which);
        break;
    case SDL_CONTROLLERDEVICEADDED:
        std::cout << "Gamepad connected: " << event->cdevice.which << std::endl;
        addGamepad(event->cdevice.which);
        break;
    }
};

bool Input::getKey(SDL_Keycode keycode)
{
    SDL_Scancode mappedCode = SDL_GetScancodeFromKey(keycode);
    return currKeyStates[mappedCode];
};

bool Input::keyDown(SDL_Keycode keycode)
{
    SDL_Scancode mappedCode = SDL_GetScancodeFromKey(keycode);
    return currKeyStates[mappedCode] && !lastKeyStates[mappedCode];
};

bool Input::keyUp(SDL_Keycode keycode)
{
    SDL_Scancode mappedCode = SDL_GetScancodeFromKey(keycode);
    return !currKeyStates[mappedCode] && lastKeyStates[mappedCode];
};

bool Input::getMouseButton(MouseButton button)
{
    return SDL_BUTTON(button) & currMouseState;
}

bool Input::mouseDown(MouseButton button)
{
    return (SDL_BUTTON(button) & currMouseState) && !((SDL_BUTTON(button) & lastMouseState));
};

bool Input::mouseUp(MouseButton button)
{
    return !(SDL_BUTTON(button) & currMouseState) && ((SDL_BUTTON(button) & lastMouseState));
};

glm::vec2 Input::getMousePosition()
{
    return glm::vec2(mouseX, mouseY);
}

bool Input::getGamepadButton(Gamepad::Button button, Uint8 controllerId)
{
    int mappedId = controllerId - 1;
    if (gamepads[mappedId] != nullptr)
    {
        return gamepads[mappedId]->getButton(button);
    }

    return false;
}

Sint16 Input::getGamepadAxis(Gamepad::Axis axis, Uint8 controllerId)
{
    int mappedId = controllerId - 1;
    if (gamepads[mappedId] != nullptr)
    {
        return gamepads[mappedId]->getAxis(axis);
    }

    return 0;
}

Uint8 Input::gamepadButtonDown(Gamepad::Button button, Uint8 controllerId)
{
    int mappedId = controllerId - 1;
    if (controllerId && gamepads[mappedId] != nullptr)
    {
        return gamepads[mappedId]->buttonDown(button);
    }
    if (!controllerId)
    {
        for (int i = 0; i < MAX_GAMEPADS_NUM; i++)
        {
            if (gamepads[i] != nullptr && gamepads[i]->buttonDown(button))
            {
                return i + 1;
            }
        }
    }
    return 0;
}

Uint8 Input::gamepadButtonUp(Gamepad::Button button, Uint8 controllerId)
{
    int mappedId = controllerId - 1;
    if (controllerId && gamepads[mappedId] != nullptr)
    {
        return gamepads[mappedId]->buttonUp(button);
    }
    if (!controllerId)
    {
        for (int i = 0; i < MAX_GAMEPADS_NUM; i++)
        {
            if (gamepads[i] != nullptr && gamepads[i]->buttonUp(button))
            {
                return i + 1;
            }
        }
    }
    return 0;
}

void Input::Destroy()
{
    delete[] lastKeyStates;

    for (int i = 0; i < MAX_GAMEPADS_NUM; i++)
    {
        if (gamepads[i] != nullptr)
        {
            SDL_GameControllerClose(gamepads[i]->controllerRef);
            delete gamepads[i];
        }
    }
};

void Input::processGamepadButtonEvent(SDL_ControllerButtonEvent event)
{
    Gamepad *pad = nullptr;
    for (int i = 0; i < MAX_GAMEPADS_NUM; i++)
    {
        if (gamepads[i]->joyId == event.which)
        {
            pad = gamepads[i];
            break;
        }
    }

    if (pad == nullptr)
    {
        std::cout << "Could not find gamepad connected to device id." << std::endl;
        return;
    }

    pad->setButton(event.button, (event.state == SDL_PRESSED));
}

void Input::processGamepadAxisEvent(SDL_ControllerAxisEvent event)
{
    Gamepad *pad = nullptr;
    for (int i = 0; i < MAX_GAMEPADS_NUM; i++)
    {
        if (gamepads[i]->joyId == event.which)
        {
            pad = gamepads[i];
            break;
        }
    }

    if (pad == nullptr)
    {
        std::cout << "Could not find gamepad connected to device id." << std::endl;
        return;
    }

    pad->setAxis(event.axis, event.value);
}

bool Input::addGamepad(SDL_JoystickID id)
{
    for (int i = 0; i < MAX_GAMEPADS_NUM; i++)
    {
        if (gamepads[i] == nullptr)
        {
            SDL_GameController *pad = SDL_GameControllerOpen(id);
            gamepads[i] = new Gamepad(pad);
            return true;
        }
    }
    std::cout << "Could not add new gamepad: limit reached" << std::endl;
    return false;
}

bool Input::removeGamepad(SDL_JoystickID id)
{
    for (int i = 0; i < MAX_GAMEPADS_NUM; i++)
    {
        if (gamepads[i]->joyId == id)
        {
            SDL_GameControllerClose(gamepads[i]->controllerRef);
            delete gamepads[i];
            gamepads[i] = nullptr;
            return true;
        }
    }
    return false;
}