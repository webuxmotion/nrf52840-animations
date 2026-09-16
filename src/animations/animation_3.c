#include "animation_3.h"
#include <zephyr/display/cfb.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

float xpos = 0.0f;
float ypos = 0.0f;
float zpos = 0.0f; 
float vx = 0.0f;
float vy = 0.0f;
float vz = 0.0f;
float friction = 0.95f;
float fl = 250.0f;
float boxSize = 40.0f;

void anim3_draw(const struct device *display, float dt, int width, int height, ControlsState buttons) {
  int vpX = width / 2;
  int vpY = height / 2;

  xpos += vx;
  ypos += vy;
  zpos += vz;
  vx *= friction;
  vy *= friction;
  vz *= friction;

  if (zpos > -fl) {
    float scale = fl / (fl + zpos);

    struct cfb_position p0 = {
      .x = (int)(vpX + xpos * scale + boxSize / 2.0f * scale),
      .y = (int)(vpY + ypos * scale + boxSize / 2.0f * scale)
    };
    struct cfb_position p1 = {
      .x = (int)(vpX + xpos * scale + boxSize / 2.0f * scale),
      .y = (int)(vpY + ypos * scale - boxSize / 2.0f * scale)
    };
    struct cfb_position p2 = {
      .x = (int)(vpX + xpos * scale - boxSize / 2.0f * scale),
      .y = (int)(vpY + ypos * scale - boxSize / 2.0f * scale)
    };
    struct cfb_position p3 = {
      .x = (int)(vpX + xpos * scale - boxSize / 2.0f * scale),
      .y = (int)(vpY + ypos * scale + boxSize / 2.0f * scale)
    };

    cfb_draw_line(display, &p0, &p1);
    cfb_draw_line(display, &p1, &p2);
    cfb_draw_line(display, &p2, &p3);
    cfb_draw_line(display, &p3, &p0);

    if (buttons.btn_1) {
      vx -= 0.5f;
    } else if (buttons.btn_2) {
      vx += 0.5f;
    }
  }
}
