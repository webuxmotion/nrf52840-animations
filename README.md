# Zephyr RTOS SH1106 OLED "Hello World" Project

This guide takes you from a completely empty project folder to running a "Hello World" text on a 1.3" (128x64) OLED display using the SPI interface.

## Prerequisites
Ensure you have the Zephyr development environment installed and the `west` tool configured.

```
west build -p -b nrf52840dk/nrf52840
west flash
west build && west flash

ls /dev/tty.usbmodem*
screen /dev/tty.usbmodem0010502028451 115200

To close the session and return to the regular command line:Press Ctrl + A.
Immediately after, press the K key (stands for "kill").
A prompt will appear at the bottom: Really kill this window? [y/n]. Press Y.
```

---

## Supported Hardware & Where to Buy

This codebase is specifically tailored for a **1.3-inch 128x64 OLED display** running on the **Sinowealth SH1106** controller chip. 

> ⚠️ **Important Note:** You must use a **7-pin (or 8-pin) SPI variant** of the module. 4-pin modules that only support I2C will not work with this specific configuration.

### Recommended Purchase Links:
* **Official Brand:** [Waveshare 1.3inch OLED Module (B)](https://waveshare.com) — Global shipping from the manufacturer.
* **Local Distributor (Ukraine):** [Arduino.ua - OLED Display 1.3" I2C/SPI 128x64 (Blue)](https://arduino.ua) — Fast shipping within Ukraine.
* **Generic Alternatives:** Search for **"7-pin 1.3 inch SPI OLED SH1106"** on Amazon, AliExpress, or eBay. Ensure the listing explicitly specifies the **SH1106** driver, as some 1.3" panels mistakenly use SSD1306 chips, which require different software configurations.

---

## Step 1: Create the Project Structure

Open your terminal, navigate to your empty project folder, and create the following directory layout:

```bash
mkdir src
```

Your project folder structure should look like this:
```text
my_oled_project/
└── src/
```

---

## Step 2: Create the Files & Add Code

Create the following **four essential files** and copy the respective code into each of them:

### 1. `CMakeLists.txt` (Root Folder)
```cmake
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS \$ENV{ZEPHYR_BASE})
project(oled_hello_world)

target_sources(app PRIVATE src/main.c)
```

### 2. `prj.conf` (Root Folder)
```cfg
CONFIG_GPIO=y
CONFIG_SPI=y

CONFIG_DISPLAY=y
CONFIG_SSD1306=y

CONFIG_CHARACTER_FRAMEBUFFER=y
CONFIG_HEAP_MEM_POOL_SIZE=4096

CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y
CONFIG_LOG=y

CONFIG_MAIN_STACK_SIZE=2048
CONFIG_ASSERT=y
```

### 3. `app.overlay`
This file overrides your microcontroller's default pin settings and configures the SH1106 screen.

```dts
#include <zephyr/dt-bindings/gpio/gpio.h>

/ {
	chosen {
		/* Вказуємо Zephyr використовувати саме наш пристрій як головний дисплей */
		zephyr,display = &oled;
	};
};

&pinctrl {
	spi1_default: spi1_default {
		group1 {
			psels = <NRF_PSEL(SPIM_SCK, 1, 5)>,
				<NRF_PSEL(SPIM_MOSI, 1, 6)>;
		};
	};

	spi1_sleep: spi1_sleep {
		group1 {
			psels = <NRF_PSEL(SPIM_SCK, 1, 5)>,
				<NRF_PSEL(SPIM_MOSI, 1, 6)>;
			low-power-enable;
		};
	};
};

&spi1 {
	compatible = "nordic,nrf-spim";
	status = "okay";
	/* Явно зв'язуємо конфігурацію пінів з периферійним модулем */
	pinctrl-0 = <&spi1_default>;
	pinctrl-1 = <&spi1_sleep>;
	pinctrl-names = "default", "sleep";
	cs-gpios = <&gpio0 4 GPIO_ACTIVE_LOW>;

	oled: sh1106@0 {
		compatible = "sinowealth,sh1106";
		reg = <0>;
		spi-max-frequency = <8000000>;
		width = <128>;
		height = <64>;
		segment-offset = <2>;
		page-offset = <0>;
		display-offset = <0>;
		multiplex-ratio = <63>;
		prechargep = <0x22>;
		segment-remap;
		com-invdir;
		data-cmd-gpios = <&gpio0 29 GPIO_ACTIVE_HIGH>;
		reset-gpios = <&gpio0 30 GPIO_ACTIVE_LOW>;
	};
};
```

### 4. `src/main.c` (Src Folder)
```c
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

	cfb_draw_text(display, "Hello World!", x_pos, y_pos);

	cfb_framebuffer_finalize(display);
	LOG_INF("Hello World sent to display.");

	while (1) {
		k_sleep(K_FOREVER);
	}

	return 0;
}
```

---

## Step 3: Physical Wiring (7 Wires)

Connect your 7-pin OLED module to your Nordic development board using the following mapping:

| OLED Board Pin Label | Function | Target MCU Pin Location |
| :--- | :--- | :--- |
| **VCC** | Power Supply (3.3V) | **3.3V** |
| **GND** | Ground | **GND** |
| **CLK / SCL / D0** | SPI Clock | **P1.05** |
| **DIN / SDA / D1** | SPI MOSI Data | **P1.06** |
| **CS** | Chip Select | **P0.04** |
| **D/C / DC** | Data / Command Selection | **P0.29** |
| **RES / RST** | Hardware Reset | **P0.30** |

---

## Step 4: Build and Flash

1. Run the compilation command from the root of your project directory. Replace `<your_board_target>` with your physical hardware profile (e.g., `nrf52840dk_nrf52840`):
   ```bash
   west build -b <your_board_target>
   ```

2. Flash the compiled image onto the connected target microchip:
   ```bash
   west flash
   ```
