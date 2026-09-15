#ifndef RENDER_H_
#define RENDER_H_

#include <zephyr/device.h>
#include <stdint.h>

void render_init_lines(uint16_t width, uint16_t height);
void render_frame_all(const struct device *display, float angle, float dt, uint16_t width, uint16_t height);
void render_frame(const struct device *display, float angle);

#endif /* RENDER_H_ */
