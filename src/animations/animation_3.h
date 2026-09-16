#ifndef ANIMATION_3_H_
#define ANIMATION_3_H_

#include <zephyr/device.h>
#include "../controls.h"

void anim3_draw(const struct device *display, float dt, int width, int height, ControlsState buttons);

#endif /* ANIMATION_3_H_ */
