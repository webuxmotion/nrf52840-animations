#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <stdio.h>

#include "controls.h"
#include "render.h"

static const struct device *const display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
K_SEM_DEFINE(display_sem, 0, 1);

void animation_timer_handler(struct k_timer *dummy) {
	k_sem_give(&display_sem);
}
K_TIMER_DEFINE(anim_timer, animation_timer_handler, NULL);

int main(void) {
	if (!device_is_ready(display) || cfb_framebuffer_init(display)) {
		return -EIO;
	}

  int64_t last_time = k_uptime_get();
  int64_t current_time;

	cfb_framebuffer_invert(display);

	init_controls();

	uint16_t width = cfb_get_display_parameter(display, CFB_DISPLAY_WIDTH);
	uint16_t height = cfb_get_display_parameter(display, CFB_DISPLAY_HEIGHT);

  float angle = 0.0f;
  float rotate_speed = 0.0f;

  render_init_lines(width, height);

	k_timer_start(&anim_timer, K_NO_WAIT, K_MSEC(20)); 

	while (1) {
		k_sem_take(&display_sem, K_FOREVER);

    current_time = k_uptime_get();
    float dt = (current_time - last_time) / 1000.0f;
    last_time = current_time;

    get_controls_snapshot(&rotate_speed);

		cfb_framebuffer_clear(display, false);

    cfb_framebuffer_set_font(display, 2);
    cfb_draw_text(display, "webuxmotion", 2, 20);

		render_frame_all(display, angle, dt, width, height);
    render_draw(display, dt, height);

    angle += rotate_speed * dt;

		cfb_framebuffer_finalize(display);
	}
	return 0;
}