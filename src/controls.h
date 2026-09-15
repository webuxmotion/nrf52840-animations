#ifndef CONTROLS_H_
#define CONTROLS_H_

// Захищаємо controls.c і даємо йому всі типи ядра Zephyr тут!
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    bool btn_1;
    bool btn_2;
    bool btn_3;
    bool btn_4;
} ControlsState;

void init_controls(void);
void get_buttons_snapshot(ControlsState *state);

#endif /* CONTROLS_H_ */
