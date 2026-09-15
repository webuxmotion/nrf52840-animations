#include "render.h"
#include <zephyr/display/cfb.h>
#include <math.h>
#include <string.h>

#include "animations/animation_1.h"
#include "animations/animation_2.h"

void render_draw(const struct device *display, float dt, ControlsState buttons, int width, int height) {
  //anim1_draw(display, dt, width, height, buttons);
  anim2_draw(display, dt, width, height, buttons);
};