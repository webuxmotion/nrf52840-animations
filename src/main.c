#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device *const display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

int main(void)
{
	uint8_t font_width;
	uint8_t font_height;
	int x_pos;
	int y_pos;

	if (!device_is_ready(display)) {
		LOG_ERR("Display device not ready");
		return -ENODEV;
	}

	if (cfb_framebuffer_init(display)) {
		LOG_ERR("Framebuffer initialization failed");
		return -EIO;
	}

	cfb_framebuffer_clear(display, true);
	display_blanking_off(display);

	cfb_framebuffer_set_font(display, 0);
	cfb_get_font_size(display, 0, &font_width, &font_height);

	cfb_framebuffer_clear(display, false);
  cfb_framebuffer_invert(display);

	x_pos = (128 - (12 * font_width)) / 2;
	y_pos = (64 - font_height) / 2;

	if (x_pos < 0) x_pos = 0;
	if (y_pos < 0) y_pos = 0;

	cfb_draw_text(display, "Hello world!", x_pos, y_pos);

	cfb_framebuffer_finalize(display);
	LOG_INF("Hello World sent to display.");

	while (1) {
		k_sleep(K_FOREVER);
	}

	return 0;
}
