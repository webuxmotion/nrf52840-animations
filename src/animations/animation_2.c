#include "animation_2.h"
#include <zephyr/display/cfb.h>
#include <math.h>

float xpos = 0.0f;
float ypos = 0.0f;
float zpos = 150.0f;
int fl = 250;
int vpX = 50;
int vpY = 50;
int radius = 20;

float angle = 0.0f;

void anim2_draw(const struct device *display, float dt, int width, int height, ControlsState buttons) {
  vpX = width / 2;
  vpY = height / 2;

  if (buttons.btn_2) {
    zpos += 500 * dt;
  } else if (buttons.btn_4) {
    zpos -= 500 * dt;
  }

  float x = cosf(angle) * 100;
  float y = sinf(angle) * 100;

  float scale = fl / (fl + zpos + y);
  float scaled_radius = radius * scale;

  struct cfb_position position = {
    x: vpX + (xpos + x) * scale,
    y: vpY + ypos * scale
  };

  angle += 0.1f;

  cfb_draw_circle(display, &position, scaled_radius);
}
