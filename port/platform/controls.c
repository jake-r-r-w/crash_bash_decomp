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

/* --- the sticks ---
 *
 * The game takes a stick for a d-pad direction once one axis is past 0x24 or 0xDB, 92 from the middle each way, nearly three quarters
 * of the way out (pad_read), as on the PlayStation, and only then moves a hero, by the stick's angle. On a diagonal that's all the way
 * out. Players found that a lot (GitHub issue 1, 2026-11-10). With a deadzone of the player's own, a stick is pushed further out from the
 * middle along its own angle, by as much as puts the game's line at the deadzone: the same distance out at every angle, and never past
 * the square the game reads. It's done to each player's own controller before the game has it, so online each machine's players have
 * their own, and every machine's game sees the same. */
#define STICK_GATE (92.0f / 127.5f)

static unsigned char stick_byte(float v) {
    int b = (int)SDL_lroundf(127.5f + v * 127.5f);

    return (unsigned char)(b < 0 ? 0 : b > 255 ? 255 : b);
}

void controls_stick(int x, int y, int deadzone, unsigned char out[2]) {
    float fx = SDL_clamp(x / 32767.0f, -1.0f, 1.0f), fy = SDL_clamp(y / 32767.0f, -1.0f, 1.0f), m, side, k;

    if (deadzone <= 0) {
        out[0] = (unsigned char)((x + 32768) >> 8);
        out[1] = (unsigned char)((y + 32768) >> 8);
        return;
    }
    m = SDL_sqrtf(fx * fx + fy * fy);
    if (m < 1e-6f) {
        out[0] = out[1] = 128;
        return;
    }
    /* how far out the square's edge is, along the stick's way, for each of the stick's own */
    side = SDL_max(SDL_fabsf(fx), SDL_fabsf(fy)) / m;
    k = STICK_GATE / (deadzone / 100.0f * side);
    if (m * k * side > 1.0f) {
        k = 1.0f / (m * side);
    }
    out[0] = stick_byte(fx * k);
    out[1] = stick_byte(fy * k);
}

/* Whether the game would take a stick held there for a direction: what pad_poll makes of it after a few frames (each its own half and
   the last's, rounded down), and pad_read's line. */
static int stick_is_direction(const unsigned char at[2]) {
    int v[2] = { 128, 128 }, i, f;

    for (f = 0; f < 30; f++) {
        for (i = 0; i < 2; i++) {
            v[i] = (v[i] + at[i]) >> 1;
        }
    }
    return v[0] < 0x25 || v[0] >= 0xDB || v[1] < 0x25 || v[1] >= 0xDB;
}

int controls_check_sticks(void) {
    unsigned char at[2];
    int dz, a, x;

    /* the game's own, as it always was */
    for (x = -32768; x <= 32767; x += 97) {
        controls_stick(x, -x - 1, 0, at);
        if (at[0] != (unsigned char)((x + 32768) >> 8) || at[1] != (unsigned char)((-x - 1 + 32768) >> 8)) {
            SDL_Log("sticks: the game's own deadzone changed at %d", x);
            return 0;
        }
    }
    for (dz = 10; dz <= STICK_DEADZONE_MOST; dz += 10) {
        for (a = 0; a < 360; a++) {
            float ux = SDL_cosf(a * SDL_PI_F / 180.0f), uy = SDL_sinf(a * SDL_PI_F / 180.0f);
            float in = dz / 100.0f * 0.96f, out = SDL_min(1.0f, dz / 100.0f * 1.04f);
            float got;

            controls_stick((int)(ux * in * 32767.0f), (int)(uy * in * 32767.0f), dz, at);
            if (stick_is_direction(at)) {
                SDL_Log("sticks: at %d%% a stick %.2f out at %d degrees is a direction", dz, in, a);
                return 0;
            }
            controls_stick((int)(ux * out * 32767.0f), (int)(uy * out * 32767.0f), dz, at);
            if (!stick_is_direction(at)) {
                SDL_Log("sticks: at %d%% a stick %.2f out at %d degrees isn't a direction", dz, out, a);
                return 0;
            }
            /* and the way it points, which a hero goes by */
            got = SDL_atan2f(at[1] - 127.5f, at[0] - 127.5f) * 180.0f / SDL_PI_F;
            got = SDL_fmodf(got - a + 540.0f, 360.0f) - 180.0f;
            if (SDL_fabsf(got) > 2.5f) {
                SDL_Log("sticks: at %d%% a stick at %d degrees points %.1f degrees away", dz, a, got);
                return 0;
            }
        }
    }
    /* the far corner, past where anything's pushed out, stays in the square */
    controls_stick(32767, 32767, 10, at);
    return at[0] == 255 && at[1] == 255;
}
