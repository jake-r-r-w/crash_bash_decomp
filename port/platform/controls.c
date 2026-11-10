/* The game's buttons and what plays them (controls.h): their defaults, names, and choosing a new key or button for one. */
#include <SDL3/SDL.h>
#include "controls.h"
#include "platform.h"

static const struct {
    const char *name, *file;
    int bit;
    SDL_Scancode key;
    int button;
} Controls[NUM_CONTROLS] = {
    { "CROSS", "cross", PAD_CROSS, SDL_SCANCODE_Z, SDL_GAMEPAD_BUTTON_SOUTH },
    { "CIRCLE", "circle", PAD_CIRCLE, SDL_SCANCODE_X, SDL_GAMEPAD_BUTTON_EAST },
    { "SQUARE", "square", PAD_SQUARE, SDL_SCANCODE_A, SDL_GAMEPAD_BUTTON_WEST },
    { "TRIANGLE", "triangle", PAD_TRIANGLE, SDL_SCANCODE_S, SDL_GAMEPAD_BUTTON_NORTH },
    { "START", "start", PAD_START, SDL_SCANCODE_RETURN, SDL_GAMEPAD_BUTTON_START },
    { "SELECT", "select", PAD_SELECT, SDL_SCANCODE_RSHIFT, SDL_GAMEPAD_BUTTON_BACK },
    { "L1", "l1", PAD_L1, SDL_SCANCODE_Q, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER },
    { "R1", "r1", PAD_R1, SDL_SCANCODE_W, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER },
    { "L2", "l2", PAD_L2, SDL_SCANCODE_1, BUTTON_LEFT_TRIGGER },
    { "R2", "r2", PAD_R2, SDL_SCANCODE_2, BUTTON_RIGHT_TRIGGER },
    { "UP", "up", PAD_UP, SDL_SCANCODE_UP, SDL_GAMEPAD_BUTTON_DPAD_UP },
    { "DOWN", "down", PAD_DOWN, SDL_SCANCODE_DOWN, SDL_GAMEPAD_BUTTON_DPAD_DOWN },
    { "LEFT", "left", PAD_LEFT, SDL_SCANCODE_LEFT, SDL_GAMEPAD_BUTTON_DPAD_LEFT },
    { "RIGHT", "right", PAD_RIGHT, SDL_SCANCODE_RIGHT, SDL_GAMEPAD_BUTTON_DPAD_RIGHT },
};

const char *control_name(int control) {
    return control >= 0 && control < NUM_CONTROLS ? Controls[control].name : "";
}

const char *control_file_id(int control) {
    return control >= 0 && control < NUM_CONTROLS ? Controls[control].file : "";
}

int control_bit(int control) {
    return control >= 0 && control < NUM_CONTROLS ? Controls[control].bit : 0;
}

void controls_default_keys(int keys[NUM_CONTROLS]) {
    int i;

    for (i = 0; i < NUM_CONTROLS; i++) {
        keys[i] = Controls[i].key;
    }
}

void controls_default_buttons(int buttons[NUM_CONTROLS]) {
    int i;

    for (i = 0; i < NUM_CONTROLS; i++) {
        buttons[i] = Controls[i].button;
    }
}

void controls_bind(int binds[NUM_CONTROLS], int control, int code) {
    int i;

    if (control < 0 || control >= NUM_CONTROLS) {
        return;
    }
    for (i = 0; i < NUM_CONTROLS; i++) {
        if (i != control && binds[i] == code) {
            binds[i] = binds[control];
        }
    }
    binds[control] = code;
}

const char *controls_key_id(int key) {
    return SDL_GetScancodeName((SDL_Scancode)key);
}

int controls_key_from_id(const char *id) {
    SDL_Scancode key = SDL_GetScancodeFromName(id);

    return key == SDL_SCANCODE_UNKNOWN ? -1 : (int)key;
}

const char *controls_button_id(int button) {
    if (button == BUTTON_LEFT_TRIGGER) {
        return SDL_GetGamepadStringForAxis(SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
    }
    if (button == BUTTON_RIGHT_TRIGGER) {
        return SDL_GetGamepadStringForAxis(SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
    }
    return SDL_GetGamepadStringForButton((SDL_GamepadButton)button);
}

int controls_button_from_id(const char *id) {
    SDL_GamepadButton button = SDL_GetGamepadButtonFromString(id);

    if (button != SDL_GAMEPAD_BUTTON_INVALID) {
        return (int)button;
    }
    if (SDL_GetGamepadAxisFromString(id) == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
        return BUTTON_LEFT_TRIGGER;
    }
    if (SDL_GetGamepadAxisFromString(id) == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
        return BUTTON_RIGHT_TRIGGER;
    }
    return -1;
}

/* SDL's name, in 11 letters at most (a column of the CONTROLS page), and in words for the marks the game's letters haven't got */
const char *controls_key_label(int key) {
    switch (key) {
    case SDL_SCANCODE_RETURN:
        return "ENTER";
    case SDL_SCANCODE_LEFTBRACKET:
        return "L BRACKET";
    case SDL_SCANCODE_RIGHTBRACKET:
        return "R BRACKET";
    case SDL_SCANCODE_BACKSLASH:
    case SDL_SCANCODE_NONUSBACKSLASH:
        return "BACKSLASH";
    case SDL_SCANCODE_SLASH:
        return "SLASH";
    case SDL_SCANCODE_SEMICOLON:
        return "SEMICOLON";
    case SDL_SCANCODE_GRAVE:
        return "BACKQUOTE";
    case SDL_SCANCODE_NONUSHASH:
        return "HASH";
    case SDL_SCANCODE_KP_DIVIDE:
        return "KP SLASH";
    case SDL_SCANCODE_KP_MULTIPLY:
        return "KP STAR";
    default: {
        static char shorter[32];
        const char *name = SDL_GetScancodeName((SDL_Scancode)key);

        if (SDL_strncmp(name, "Keypad ", 7) == 0) {
            SDL_snprintf(shorter, sizeof(shorter), "KP %s", name + 7);
            return shorter;
        }
        return name;
    }
    }
}
