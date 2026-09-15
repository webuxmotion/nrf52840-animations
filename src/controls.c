#include "controls.h"

static const struct gpio_dt_spec btn_1_spec = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec btn_2_spec = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static const struct gpio_dt_spec btn_3_spec = GPIO_DT_SPEC_GET(DT_ALIAS(sw2), gpios);
static const struct gpio_dt_spec btn_4_spec = GPIO_DT_SPEC_GET(DT_ALIAS(sw3), gpios);

static struct gpio_callback cb_1, cb_2, cb_3, cb_4;

static volatile bool is_btn_1_pressed = false;
static volatile bool is_btn_2_pressed = false;
static volatile bool is_btn_3_pressed = false;
static volatile bool is_btn_4_pressed = false;

static void handler_btn_1(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
	is_btn_1_pressed = gpio_pin_get_dt(&btn_1_spec);
}

static void handler_btn_2(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
	is_btn_2_pressed = gpio_pin_get_dt(&btn_2_spec);
}

static void handler_btn_3(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
	is_btn_3_pressed = gpio_pin_get_dt(&btn_3_spec);
}

static void handler_btn_4(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
	is_btn_4_pressed = gpio_pin_get_dt(&btn_4_spec);
}

void init_controls(void) {
	if (device_is_ready(btn_1_spec.port)) {
		gpio_pin_configure_dt(&btn_1_spec, GPIO_INPUT);
		gpio_pin_interrupt_configure_dt(&btn_1_spec, GPIO_INT_EDGE_BOTH);
		gpio_init_callback(&cb_1, handler_btn_1, BIT(btn_1_spec.pin));
		gpio_add_callback(btn_1_spec.port, &cb_1);
	}
	if (device_is_ready(btn_2_spec.port)) {
		gpio_pin_configure_dt(&btn_2_spec, GPIO_INPUT);
		gpio_pin_interrupt_configure_dt(&btn_2_spec, GPIO_INT_EDGE_BOTH);
		gpio_init_callback(&cb_2, handler_btn_2, BIT(btn_2_spec.pin));
		gpio_add_callback(btn_2_spec.port, &cb_2);
	}
	if (device_is_ready(btn_3_spec.port)) {
		gpio_pin_configure_dt(&btn_3_spec, GPIO_INPUT);
		gpio_pin_interrupt_configure_dt(&btn_3_spec, GPIO_INT_EDGE_BOTH);
		gpio_init_callback(&cb_3, handler_btn_3, BIT(btn_3_spec.pin));
		gpio_add_callback(btn_3_spec.port, &cb_3);
	}
	if (device_is_ready(btn_4_spec.port)) {
		gpio_pin_configure_dt(&btn_4_spec, GPIO_INPUT);
		gpio_pin_interrupt_configure_dt(&btn_4_spec, GPIO_INT_EDGE_BOTH);
		gpio_init_callback(&cb_4, handler_btn_4, BIT(btn_4_spec.pin));
		gpio_add_callback(btn_4_spec.port, &cb_4);
	}
}

void get_buttons_snapshot(ControlsState *state) {
    if (state == NULL) {
        return;
    }
    state->btn_1 = is_btn_1_pressed;
    state->btn_2 = is_btn_2_pressed;
    state->btn_3 = is_btn_3_pressed;
    state->btn_4 = is_btn_4_pressed;
}
