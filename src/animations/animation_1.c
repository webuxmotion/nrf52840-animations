#include "animation_1.h"
#include <zephyr/display/cfb.h>

static float y_pixel = 0.0f;

void anim1_draw(const struct device *display, float dt, int width, int height, ControlsState buttons) {
  cfb_framebuffer_set_font(display, 0);

  if (buttons.btn_1) {
    cfb_draw_text(display, "webuxmotion", 10, (int16_t)y_pixel);
  }

  y_pixel += 100.0f * dt;

  if (y_pixel > (float)height) {
      y_pixel = -16.0f;
  }
}
