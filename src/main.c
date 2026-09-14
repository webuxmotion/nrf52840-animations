#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device *const display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

K_SEM_DEFINE(display_sem, 0, 1);

void animation_timer_handler(struct k_timer *dummy) {
	k_sem_give(&display_sem);
}
K_TIMER_DEFINE(anim_timer, animation_timer_handler, NULL);

const int speed_px_per_sec = 64;

int main(void)
{
	uint8_t font_width;
	uint8_t font_height;
	int x_pos;
	int y_pos;

  int64_t last_time = k_uptime_get();
  int64_t current_time;

	if (!device_is_ready(display)) {
		LOG_ERR("Display device not ready");
		return -ENODEV;
	}

	if (cfb_framebuffer_init(display)) {
		LOG_ERR("Framebuffer initialization failed");
		return -EIO;
	}

	cfb_framebuffer_set_font(display, 0);
	cfb_get_font_size(display, 0, &font_width, &font_height);

	cfb_framebuffer_clear(display, false);
  cfb_framebuffer_invert(display);

	x_pos = (128 - (12 * font_width)) / 2;
	y_pos = (64 - font_height) / 2;

	if (x_pos < 0) x_pos = 0;
	if (y_pos < 0) y_pos = 0;

  uint16_t width = cfb_get_display_parameter(display, CFB_DISPLAY_WIDTH);
	uint16_t height = cfb_get_display_parameter(display, CFB_DISPLAY_HEIGHT);

  k_timer_start(&anim_timer, K_NO_WAIT, K_MSEC(20));

	while (1) {
		k_sem_take(&display_sem, K_FOREVER);

    current_time = k_uptime_get();
    float dt = (current_time - last_time) / 1000.0f;
    last_time = current_time;

    cfb_framebuffer_clear(display, false);

    cfb_draw_text(display, "Hello world!", x_pos, y_pos);

    y_pos += speed_px_per_sec * dt;

    if (y_pos > height) {
      y_pos = -font_height;
    }

    cfb_framebuffer_finalize(display);
	}

	return 0;
}
