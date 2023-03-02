#include "input.h"
#include <memory>

// matches up with the InputAction enum
int keyboardInputMap[] = {
    SDL_SCANCODE_LEFT, // move block left
    SDL_SCANCODE_RIGHT, // move block right
    SDL_SCANCODE_DOWN, // move block down
    SDL_SCANCODE_Z, // rotate left
    SDL_SCANCODE_X, // rotate right
    SDL_SCANCODE_C, // hold block
};

//using SDL_GameControllerButton::*;
SDL_GameControllerButton gamepadInputMap[] = {
    SDL_CONTROLLER_BUTTON_DPAD_LEFT, // move block left
    SDL_CONTROLLER_BUTTON_DPAD_RIGHT, // move block right
    SDL_CONTROLLER_BUTTON_DPAD_DOWN, // move block down
    SDL_CONTROLLER_BUTTON_LEFTSHOULDER, // rotate left
    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, // rotate right
    SDL_CONTROLLER_BUTTON_A, // hold block
};

// only one keyboard, so keeping track of keys pressed in a static
static Uint8 prevKeystates[512] = {};
static Uint8 keystates[512] = {};

// keeps track of the "buttons" each player has pressed. 
// "buttons" are not A or X or Left shoulder, they are "Hold block" or "move right"
// I.E. things that are directly related to the game
static PlayerInput playerInputs[MAX_NUM_PLAYERS] = {};

bool IsKeyDown(int key) {
    return keystates[key] == 1;
}
bool IsKeyUp(int key) {
    return keystates[key] == 0;
}
bool IsKeyPressed(int key) {
    // is key currently down and was up last frame
    return IsKeyDown(key) && prevKeystates[key] == 0;
}
bool IsKeyReleased(int key) {
    // is key current up and was down last frame
    return IsKeyUp(key) && prevKeystates[key] == 1;
}

void PollKeyboardInputs() {
    int numkeys = 0;
    // array of size 512 where indices correspond with SDL_SCANCODE_XXXXX 
    // and 1 means the key is pressed and 0 means key is not pressed
    const Uint8* currentKeystates = SDL_GetKeyboardState(&numkeys);

    SDL_assert(numkeys == 512);

    // copy last frame's keys to our previous keystates buffer
    memcpy(prevKeystates, keystates, 512);
    // update current keystates with current frame's
    memcpy(keystates, currentKeystates, 512);


    // update actual player inputs

    // update prev inputs
    playerInputs[0].prevButtonsPressed = playerInputs[0].buttonsPressed;
    playerInputs[0].buttonsPressed = 0;
    // for each action the player could be doing, check it
    for (int inputAction = 0; inputAction < NUM_INPUT_ACTIONS; inputAction++) {
        int keyboardMapToInputAction = keyboardInputMap[inputAction];
        bool isPlayerPressingInput = IsKeyDown(keyboardMapToInputAction);
        if (isPlayerPressingInput) {
            playerInputs[0].buttonsPressed |= (1 << inputAction); // update bitfield
        }
    }
}


bool IsPlayerInputsInitialized() {
    // we've initialized inputs if the first player is activated
    return playerInputs[0].isActive;
}

int GetNumConnectedGamepads() {
    return SDL_NumJoysticks();
}

void InitInputs() {
    // this just populates the playerInputs array with data about which
    // player index is using keyboard/controller, and which player indices are active or not
    playerInputs[0].isActive = true;
    playerInputs[0].isController = false; // keyboard is always player 1
    playerInputs[0].buttonsPressed = 0;
    playerInputs[0].prevButtonsPressed = 0;
    playerInputs[0].controller = nullptr;

    int numConnectedGamepads = GetNumConnectedGamepads();
    SDL_Log("Num connected gamepads = %i\n", numConnectedGamepads);
    for (int i = 1; i < numConnectedGamepads+1 && i < MAX_NUM_PLAYERS; i++) {
        playerInputs[i].isController = true;
        playerInputs[i].isActive = true;
        playerInputs[i].buttonsPressed = 0;
        playerInputs[i].prevButtonsPressed = 0;
        SDL_GameController* gamepad = SDL_GameControllerOpen(i-1);
        playerInputs[i].controller = gamepad;
        
    }
}

void PollGamepadInputs() {
    if (!IsPlayerInputsInitialized()) {
        InitInputs();
    }
    
    // update all controller players inputs (if they are active)
    for (int i = 1; i < MAX_NUM_PLAYERS; i++) {
        if (!playerInputs[i].isActive) continue;
        // update prev buttons
        playerInputs[i].prevButtonsPressed = playerInputs[i].buttonsPressed;
        playerInputs[i].buttonsPressed = 0;
        // for each action the player could be doing, check it
        for (int inputAction = 0; inputAction < NUM_INPUT_ACTIONS; inputAction++) {
            SDL_GameControllerButton gamepadMapToInputAction = gamepadInputMap[inputAction];
            Uint8 isPressed = SDL_GameControllerGetButton(playerInputs[i].controller, gamepadMapToInputAction);
            if (isPressed) {
                playerInputs[i].buttonsPressed |= (1 << inputAction); // update bitfield
            }
        }
    }
}

void PollInputs() {
    // update internal SDL events so our next few calls operate on most recent data
    SDL_PumpEvents();
    PollKeyboardInputs();
    PollGamepadInputs();
}



bool isActionDown(InputAction button, int playerIndex) {
    return (playerInputs[playerIndex].buttonsPressed & (1 << button)) != 0;
}

bool isActionUp(InputAction button, int playerIndex) {
    return !isActionDown(button, playerIndex);
}

bool isActionPressed(InputAction button, int playerIndex) {
    bool isActionDownThisFrame = (playerInputs[playerIndex].buttonsPressed & (1 << button)) != 0;
    bool wasActionUpLastFrame = (playerInputs[playerIndex].prevButtonsPressed & (1 << button)) == 0;
    return isActionDownThisFrame && wasActionUpLastFrame;
}

bool isActionReleased(InputAction button, int playerIndex) {
    bool isActionUpThisFrame = (playerInputs[playerIndex].buttonsPressed & (1 << button)) == 0;
    bool wasActionDownLastFrame = (playerInputs[playerIndex].prevButtonsPressed & (1 << button)) != 0;
    return isActionUpThisFrame && wasActionDownLastFrame;
}