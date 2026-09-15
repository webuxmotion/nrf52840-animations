#include "animation_2.h"
#include <zephyr/display/cfb.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


static float xpos = 0.0f;
static float ypos = 0.0f;
static float zpos = 350.0f;

static const int fl = 250;
static const int radius = 50;

static float angle = 0.0f;

void anim2_draw(const struct device *display, float dt, int width, int height, ControlsState buttons) {
    int vpX = width / 2;
    int vpY = height / 2;

    if (buttons.btn_2) {
        zpos += 300.0f * dt;
    } else if (buttons.btn_4) {
        zpos -= 300.0f * dt;
    }

    if (zpos < 10.0f) {
        zpos = 10.0f;
    }

    float x = cosf(angle) * 100.0f;
    float y = sinf(angle) * 100.0f;

    float scale = (float)fl / ((float)fl + zpos + y);
    
    int16_t scaled_radius = (int16_t)(radius * scale);
    if (scaled_radius < 1) {
        scaled_radius = 1;
    }

    struct cfb_position position = {
        .x = (int16_t)(vpX + (int)((xpos + x) * scale)),
        .y = (int16_t)(vpY + (int)(ypos * scale))
    };

    angle += 4.0f * dt;
    if (angle > 4.0f * M_PI) {
        angle -= 4.0f * M_PI;
    }

    cfb_draw_circle(display, &position, scaled_radius);
}
