#ifndef ANIMATION_1_H_
#define ANIMATION_1_H_

#include <zephyr/device.h>
#include "../controls.h"

void anim1_draw(const struct device *display, float dt, int width, int height, ControlsState buttons);

#endif /* ANIMATION_1_H_ */
