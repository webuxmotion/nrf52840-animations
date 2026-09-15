#ifndef RENDER_H_
#define RENDER_H_

#include <zephyr/device.h>
#include <stdint.h>
#include "controls.h"

void render_draw(const struct device *display, float dt, ControlsState buttons, int width, int height);

#endif /* RENDER_H_ */
