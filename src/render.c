#include "render.h"
#include <zephyr/display/cfb.h>
#include <math.h>
#include <string.h>

#include "animations/animation_3.h"

void render_draw(const struct device *display, float dt, ControlsState buttons, int width, int height) {
  anim3_draw(display, dt, width, height, buttons);
};