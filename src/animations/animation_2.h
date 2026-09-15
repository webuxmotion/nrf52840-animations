#ifndef ANIMATION_2_H_
#define ANIMATION_2_H_

#include <zephyr/device.h>
#include "../controls.h"

void anim2_draw(const struct device *display, float dt, int width, int height, ControlsState buttons);

#endif /* ANIMATION_2_H_ */
