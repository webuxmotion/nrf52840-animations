#ifndef CONTROLS_H
#define CONTROLS_H

#include <zephyr/drivers/gpio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define ROTATE_SPEED          M_PI * 2

void init_controls(void);
void get_controls_snapshot(float *rotate_speed);

#endif
