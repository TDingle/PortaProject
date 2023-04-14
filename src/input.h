#pragma once
#include "SDL/SDL.h"

#define MAX_NUM_PLAYERS 2

struct PlayerInput {
    bool isController;
    bool isActive = false;
    int buttonsPressed = 0; // bitfield
    int prevButtonsPressed = 0;
    SDL_GameController* controller = nullptr;
};

enum InputAction {
    LEFT = 0,
    RIGHT,
    DOWN,
    ROTLEFT,
    ROTRIGHT,
    HOLD,
    DROP,

    NUM_INPUT_ACTIONS,
};


// should be called every frame to allow the input system to update itself
void PollInputs();

// ------ API for checking keyboard inputs --------

// "key" refers to the SDL_SCANCODE_XXXXX family of variables

// Is the key down/up on this frame?
bool IsKeyDown(int key);
bool IsKeyUp(int key);

// Was the key *just* pressed/released on this frame?
bool IsKeyPressed(int key);
bool IsKeyReleased(int key);


// ------ API for checking game inputs ----------


/*
(input map in input.cpp)

Keyboard:
arrow keys L/R/D (left, right, down) move block l/r/d
z -> rotate left
x -> rotate right
c -> hold block

Controller:
dpad L/R/D move block l/r/d
left/right shoulder buttons rotate block
A button to hold block
*/

// NOTE: KEYBOARD IS ALWAYS PLAYER 1

// returns whether the specified player (player 1 -> playerIndex 0) is pressing the specified input action on this frame
// playerIndex defaults to player 1 (index 0)
bool isActionDown(InputAction button, int playerIndex = 0);
// same as above, but if the player is NOT currently pressing the input
bool isActionUp(InputAction button, int playerIndex = 0);

// returns if the specified player (player 1 by default) has JUST pressed the specified action
// I.E. if they were not pressing the action on the previous frame, and are now pressing the action on this frame
bool isActionPressed(InputAction button, int playerIndex = 0);
// same as above but if the player just released the action
bool isActionReleased(InputAction button, int playerIndex = 0);
