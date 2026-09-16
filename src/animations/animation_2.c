#include "animation_2.h"
#include <zephyr/display/cfb.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define DEG_TO_RAD(x) ((x) * M_PI / 180.0f)


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

    typedef struct {
      float length;
      float angle;
    } PolarVector;

    PolarVector polar_vectors[] = {
      {51.193f,  DEG_TO_RAD(12.396f)},
      {54.471f,  DEG_TO_RAD(-23.377f)},
      {60.578f,  DEG_TO_RAD(-34.373f)},
      {83.501f,  DEG_TO_RAD(-53.217f)},
      {42.249f,  DEG_TO_RAD(-73.653f)},
      {74.812f,  DEG_TO_RAD(-80.854f)}
    };

    typedef struct {
      float x;
      float y;
    } Vector;

    Vector vectors[6];  
    Vector mirrored_vectors[6];
    struct cfb_position positions[6];
    struct cfb_position mirrored_positions[6];

    for (int i = 0; i < 6; i++) {
      vectors[i].x = (int16_t)(cosf(polar_vectors[i].angle + rotation) * polar_vectors[i].length / scaleF);
      vectors[i].y = (int16_t)(sinf(polar_vectors[i].angle + rotation) * polar_vectors[i].length / scaleF);
      mirrored_vectors[i].x = -vectors[i].x;
      mirrored_vectors[i].y = -vectors[i].y;
      positions[i].x = (int16_t)(p0.x + vectors[i].x);
      positions[i].y = (int16_t)(p0.y + vectors[i].y);
      mirrored_positions[i].x = (int16_t)(p0.x + mirrored_vectors[i].x);
      mirrored_positions[i].y = (int16_t)(p0.y + mirrored_vectors[i].y);
    }

    for (int i = 0; i < 6; i++) {
      cfb_draw_line(display, &positions[i], &mirrored_positions[5 - i]);
      if (i % 2 == 0) {
        cfb_draw_line(display, &positions[i], &positions[i + 1]);
        cfb_draw_line(display, &mirrored_positions[i], &mirrored_positions[i + 1]);
      }
    }

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
