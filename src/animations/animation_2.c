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
static float rotation = 0.0f;
static float scaleF = 2.5f;

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

    //cfb_draw_circle(display, &position, scaled_radius);

    struct cfb_position p0 = {
        .x = (int16_t)vpX,
        .y = (int16_t)vpY
    };

    float len1 = 51.193f;
    float ang1 = 12.396f * M_PI / 180.0f;

    float len2 = 54.471f;
    float ang2 = -23.377 * M_PI / 180.0f;

    float len3 = 60.578f;
    float ang3 = -34.373 * M_PI / 180.0f;

    float len4 = 83.501f;
    float ang4 = -53.217 * M_PI / 180.0f;

    float len5 = 42.249f;
    float ang5 = -73.653 * M_PI / 180.0f;

    float len6 = 74.812f;
    float ang6 = -80.854 * M_PI / 180.0f;

        // point 1
    struct cfb_position v1 = {
        .x = (int16_t)(cosf(ang1 + rotation) * len1 / scaleF),
        .y = (int16_t)(sinf(ang1 + rotation) * len1 / scaleF)
    };
    struct cfb_position p1 = {
        .x = (int16_t)(p0.x + v1.x),
        .y = (int16_t)(p0.y + v1.y)
    };

    // point 2
    struct cfb_position v2 = {
        .x = (int16_t)(cosf(ang2 + rotation) * len2 / scaleF),
        .y = (int16_t)(sinf(ang2 + rotation) * len2 / scaleF)
    };
    struct cfb_position p2 = {
        .x = (int16_t)(p0.x + v2.x),
        .y = (int16_t)(p0.y + v2.y)
    };

    // point 3
    struct cfb_position v3 = {
        .x = (int16_t)(cosf(ang3 + rotation) * len3 / scaleF),
        .y = (int16_t)(sinf(ang3 + rotation) * len3 / scaleF)
    };
    struct cfb_position p3 = {
        .x = (int16_t)(p0.x + v3.x),
        .y = (int16_t)(p0.y + v3.y)
    };

    // point 4
    struct cfb_position v4 = {
        .x = (int16_t)(cosf(ang4 + rotation) * len4 / scaleF),
        .y = (int16_t)(sinf(ang4 + rotation) * len4 / scaleF)
    };
    struct cfb_position p4 = {
        .x = (int16_t)(p0.x + v4.x),
        .y = (int16_t)(p0.y + v4.y)
    };

    // point 5
    struct cfb_position v5 = {
        .x = (int16_t)(cosf(ang5 + rotation) * len5 / scaleF),
        .y = (int16_t)(sinf(ang5 + rotation) * len5 / scaleF)
    };
    struct cfb_position p5 = {
        .x = (int16_t)(p0.x + v5.x),
        .y = (int16_t)(p0.y + v5.y)
    };

    // point 6
    struct cfb_position v6 = {
        .x = (int16_t)(cosf(ang6 + rotation) * len6 / scaleF),
        .y = (int16_t)(sinf(ang6 + rotation) * len6 / scaleF)
    };
    struct cfb_position p6 = {
        .x = (int16_t)(p0.x + v6.x),
        .y = (int16_t)(p0.y + v6.y)
    };

    struct cfb_position v1m = {
        .x = -v1.x,
        .y = -v1.y
    };
    struct cfb_position v2m = {
        .x = -v2.x,
        .y = -v2.y
    };
    struct cfb_position v3m = {
        .x = -v3.x,
        .y = -v3.y
    };
    struct cfb_position v4m = {
        .x = -v4.x,
        .y = -v4.y
    };
    struct cfb_position v5m = {
        .x = -v5.x,
        .y = -v5.y
    };
    struct cfb_position v6m = {
        .x = -v6.x,
        .y = -v6.y
    };

    struct cfb_position p1m = {
        .x = p0.x + v1m.x,
        .y = p0.y + v1m.y
    };
    struct cfb_position p2m = {
        .x = p0.x + v2m.x,
        .y = p0.y + v2m.y
    };
    struct cfb_position p3m = {
        .x = p0.x + v3m.x,
        .y = p0.y + v3m.y
    };
    struct cfb_position p4m = {
        .x = p0.x + v4m.x,
        .y = p0.y + v4m.y
    };
    struct cfb_position p5m = {
        .x = p0.x + v5m.x,
        .y = p0.y + v5m.y
    };
    struct cfb_position p6m = {
        .x = p0.x + v6m.x,
        .y = p0.y + v6m.y
    };

    cfb_draw_line(display, &p1, &p2);
    cfb_draw_line(display, &p3, &p4);
    cfb_draw_line(display, &p5, &p6);

    cfb_draw_line(display, &p1m, &p2m);
    cfb_draw_line(display, &p3m, &p4m);
    cfb_draw_line(display, &p5m, &p6m);

    cfb_draw_line(display, &p1, &p6m);
    cfb_draw_line(display, &p2, &p5m);
    cfb_draw_line(display, &p3, &p4m);
    cfb_draw_line(display, &p4, &p3m);
    cfb_draw_line(display, &p5, &p2m);
    cfb_draw_line(display, &p6, &p1m);

    if (buttons.btn_3) {
      rotation += 0.1f;
    }
    if (buttons.btn_2) {
      scaleF += 0.1;
    } else if (buttons.btn_4) {
      scaleF -= 0.1;
      if (scaleF < 0.1) {
        scaleF = 0.1;
      }
    }
}
