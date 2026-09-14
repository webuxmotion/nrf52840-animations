# Zephyr RTOS SH1106 OLED "Hello World" Project

This guide takes you from a completely empty project folder to running a "Hello World" text on a 1.3" (128x64) OLED display using the SPI interface.

## Prerequisites
Ensure you have the Zephyr development environment installed and the `west` tool configured.

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
mkdir boards
```

Your project folder structure should look like this:
```text
my_oled_project/
├── boards/
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
# Enable GPIO and SPI
CONFIG_GPIO=y
CONFIG_SPI=y

# Enable Display subsystem and the SH1106 Driver
CONFIG_DISPLAY=y
CONFIG_SH1106=y

# Enable Character Framebuffer (CFB) for simple text rendering
CONFIG_CHARACTER_FRAMEBUFFER=y
```

### 3. `boards/app.overlay` (Boards Folder)
This file overrides your microcontroller's default pin settings and configures the SH1106 screen.

```dts
#include <zephyr/dt-bindings/gpio/gpio.h>

/ {
	chosen {
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
#include <zephyr/display/cfb.h>

int main(void)
{
	const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

	if (!device_is_ready(display_dev)) {
		printk("Error: Display device not ready\n");
		return -1;
	}

	if (cfb_framebuffer_init(display_dev)) {
		printk("Error: Framebuffer initialization failed\n");
		return -1;
	}

	// Clear framebuffer and select default font
	cfb_framebuffer_clear(display_dev, true);
	cfb_select_font(display_dev, 0);
	
	// Print text lines
	cfb_print(display_dev, "Hello World!", 0, 0);
	cfb_print(display_dev, "Zephyr RTOS", 0, 16);
	
	// Push local buffer to the physical OLED screen
	cfb_framebuffer_finalize(display_dev);

	printk("Display updated successfully.\n");
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
