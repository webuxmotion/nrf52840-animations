# Zephyr RTOS SH1106 OLED "Hello World" Project

This guide will take you from a completely empty project folder to running a "Hello World" text on a 1.3" (128x64) SH1106 OLED display using the SPI interface.

## Prerequisites
Ensure you have the Zephyr development environment installed and the `west` tool configured.

---

## Step 1: Create the Project Structure

Open your terminal, navigate to your empty project folder, and create the following directory layout:

```bash
mkdir src
mkdir boards
```

Your project folder should now look like this:
```text
my_oled_project/
├── boards/
└── src/
```

---

## Step 2: Create the Configuration Files

You need to create **four essential files**. Copy and paste the respective code into each file.

### 1. `CMakeLists.txt` (Root Folder)
Create this file in the main project directory. It tells CMake how to build the application.

```cmake
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS \$ENV{ZEPHYR_BASE})
project(oled_hello_world)

target_sources(app PRIVATE src/main.c)
```

### 2. `prj.conf` (Root Folder)
Create this file in the main project directory. It enables the hardware drivers and framebuffer subsystems inside Zephyr.

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
Create this file inside the `boards` directory. **This is your exact hardware configuration.** It assigns the SPI pins and tells Zephyr that the SH1106 OLED is the main display.

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
Create this file inside the `src` directory. This is the application logic that initializes the screen and prints the text.

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
	
	// Print text at X=0, Y=0 (Line 0)
	cfb_print(display_dev, "Hello World!", 0, 0);
	cfb_print(display_dev, "Zephyr RTOS", 0, 16);
	
	// Push the local buffer to the physical OLED screen
	cfb_framebuffer_finalize(display_dev);

	printk("Display updated successfully.\n");
	return 0;
}
```

---

## Step 3: Physical Wiring (7 Wires)

Connect your 7-pin OLED module to your Nordic development board according to this table:

| OLED Pin | Description | Target MCU Pin |
| :--- | :--- | :--- |
| **VCC** | Power Supply | **3.3V** |
| **GND** | Ground | **GND** |
| **CLK / SCL** | SPI Clock | **P1.05** |
| **DIN / SDA** | SPI MOSI Data | **P1.06** |
| **CS** | Chip Select | **P0.04** |
| **D/C / DC** | Data / Command | **P0.29** |
| **RES / RST** | Hardware Reset | **P0.30** |

---

## Step 4: Build and Flash

1. **Build the application:** Run the build command from the root of your project directory. Replace `<your_board_target>` with your actual board name (e.g., `nrf52840dk_nrf52840`).
   ```bash
   west build -b <your_board_target>
   ```

2. **Flash the board:** Connect your development board to your computer via USB and flash the compiled binary.
   ```bash
   west flash
   ```

Once flashing is complete, the display will automatically reset and display **"Hello World!"**.
