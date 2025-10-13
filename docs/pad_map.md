# Pad Map

This document describes the pin/pad assignments for all peripherals in the user project.

## Overview

The Caravel user project area provides 38 GPIO pads (`mprj_io[37:0]`) for user connections. The first 5 pads (`mprj_io[4:0]`) are typically reserved for special functions, leaving `mprj_io[37:5]` available for user peripherals.

## Pad Allocation

| Pad Range      | Function           | Signals |
|----------------|--------------------|---------|
| mprj_io[11:5]  | SPI0 Master        | 7 pins  |
| mprj_io[18:12] | SPI1 Master        | 7 pins  |
| mprj_io[22:19] | I2C Controller     | 4 pins  |
| mprj_io[24:23] | GPIO (2 lines)     | 2 pins  |
| mprj_io[37:25] | Unused/Reserved    | 13 pins |

---

## SPI0 Master (CF_SPI) - mprj_io[11:5]

| Pad       | Signal  | Direction | Type       | Description |
|-----------|---------|-----------|------------|-------------|
| mprj_io[5]  | spi0_clk  | Output    | Push-Pull  | SPI Clock |
| mprj_io[6]  | spi0_mosi | Output    | Push-Pull  | Master Out Slave In |
| mprj_io[7]  | spi0_miso | Input     | Input      | Master In Slave Out |
| mprj_io[8]  | spi0_ss0  | Output    | Push-Pull  | Slave Select 0 |
| mprj_io[9]  | spi0_ss1  | Output    | Push-Pull  | Slave Select 1 (optional) |
| mprj_io[10] | spi0_ss2  | Output    | Push-Pull  | Slave Select 2 (optional) |
| mprj_io[11] | spi0_ss3  | Output    | Push-Pull  | Slave Select 3 (optional) |

**Configuration:**
- Output pins: `io_oeb = 0` (output enable)
- Input pins: `io_oeb = 1` (input enable)
- All outputs are push-pull configuration

---

## SPI1 Master (CF_SPI) - mprj_io[18:12]

| Pad        | Signal  | Direction | Type       | Description |
|------------|---------|-----------|------------|-------------|
| mprj_io[12] | spi1_clk  | Output    | Push-Pull  | SPI Clock |
| mprj_io[13] | spi1_mosi | Output    | Push-Pull  | Master Out Slave In |
| mprj_io[14] | spi1_miso | Input     | Input      | Master In Slave Out |
| mprj_io[15] | spi1_ss0  | Output    | Push-Pull  | Slave Select 0 |
| mprj_io[16] | spi1_ss1  | Output    | Push-Pull  | Slave Select 1 (optional) |
| mprj_io[17] | spi1_ss2  | Output    | Push-Pull  | Slave Select 2 (optional) |
| mprj_io[18] | spi1_ss3  | Output    | Push-Pull  | Slave Select 3 (optional) |

**Configuration:**
- Output pins: `io_oeb = 0` (output enable)
- Input pins: `io_oeb = 1` (input enable)
- All outputs are push-pull configuration

---

## I2C Controller (EF_I2C) - mprj_io[22:19]

| Pad        | Signal   | Direction  | Type        | Description |
|------------|----------|------------|-------------|-------------|
| mprj_io[19] | i2c_scl  | Bidir      | Open-Drain  | I2C Clock Line |
| mprj_io[20] | i2c_sda  | Bidir      | Open-Drain  | I2C Data Line |
| mprj_io[21] | i2c_scl_pu | Output   | Push-Pull   | SCL Pull-up enable (optional) |
| mprj_io[22] | i2c_sda_pu | Output   | Push-Pull   | SDA Pull-up enable (optional) |

**Configuration:**
- I2C lines (SCL, SDA) use open-drain configuration:
  - Drive 0: `io_out = 0`, `io_oeb = 0` (actively drive low)
  - Release (high-Z): `io_out = 0`, `io_oeb = 1` (input mode, external pull-up pulls high)
  - Read: Sample `io_in` value
- Pull-up enable signals are optional and depend on external hardware

**Open-Drain Implementation:**
```verilog
// Example for SCL
assign io_out[19] = 1'b0;              // Always drive 0 when enabled
assign io_oeb[19] = i2c_scl_out;       // 0 = drive low, 1 = release/hi-Z
assign i2c_scl_in = io_in[19];         // Read current line state
```

---

## GPIO (EF_GPIO8) - mprj_io[24:23]

| Pad        | Signal    | Direction | Type       | Description |
|------------|-----------|-----------|------------|-------------|
| mprj_io[23] | gpio_0    | Bidir     | Push-Pull  | GPIO Line 0 with edge-detect |
| mprj_io[24] | gpio_1    | Bidir     | Push-Pull  | GPIO Line 1 with edge-detect |

**Configuration:**
- Direction controlled by GPIO DIR register
- Input mode: `io_oeb = 1`
- Output mode: `io_oeb = 0`
- Output value from GPIO DATA register
- Input value to GPIO DATA register
- Edge-detect interrupts configurable per GPIO register map

**Example Configuration:**
```verilog
// GPIO 0 configuration
assign io_out[23] = gpio_data_out[0];
assign io_oeb[23] = ~gpio_dir[0];      // 0 = output, 1 = input
assign gpio_data_in[0] = io_in[23];

// GPIO 1 configuration
assign io_out[24] = gpio_data_out[1];
assign io_oeb[24] = ~gpio_dir[1];
assign gpio_data_in[1] = io_in[24];
```

---

## Unused Pads - mprj_io[37:25]

These pads are currently unused and should be configured as inputs with pull-downs or tied appropriately:

```verilog
// Unused pads configured as inputs
assign io_out[37:25] = 13'b0;
assign io_oeb[37:25] = 13'b1;  // All inputs
```

---

## Reserved Pads - mprj_io[4:0]

These pads are typically reserved for Caravel system functions:
- `mprj_io[0]` - JTAG or debug
- `mprj_io[1]` - JTAG or debug
- `mprj_io[2]` - JTAG or debug
- `mprj_io[3]` - JTAG or debug
- `mprj_io[4]` - User clock or special function

**Do not use these pads for user peripherals without careful consideration.**

---

## Pin Configuration Summary Table

| Peripheral | Start Pad | End Pad | Total Pins | Configuration |
|-----------|-----------|---------|------------|---------------|
| SPI0      | 5         | 11      | 7          | Push-pull outputs, 1 input |
| SPI1      | 12        | 18      | 7          | Push-pull outputs, 1 input |
| I2C       | 19        | 22      | 4          | 2 open-drain bidir, 2 outputs |
| GPIO      | 23        | 24      | 2          | Bidirectional with edge-detect |
| Unused    | 25        | 37      | 13         | Configured as inputs |
| Reserved  | 0         | 4       | 5          | System reserved |

---

## Modifying Pad Assignments

To change pad assignments:

1. Update this document with new assignments
2. Modify `user_project.v` to update the pad connections
3. Modify `user_project_wrapper.v` to reflect new connections
4. Update verification tests to match new pad assignments
5. Verify no conflicts with reserved pads

**Important Notes:**
- Always avoid `mprj_io[4:0]` for user functions
- Maintain proper spacing between peripherals for easier routing
- Consider signal integrity for high-speed signals (SPI clock)
- I2C requires external pull-ups (typically 4.7kΩ to 10kΩ)
- Open-drain configuration for I2C is critical for proper operation

---

## External Hardware Requirements

### I2C Pull-ups
The I2C interface requires external pull-up resistors on SCL and SDA lines:
- Recommended value: 4.7kΩ - 10kΩ
- Pull-up voltage: Match I/O voltage (typically 3.3V)
- Pull-ups can be on-board or external

### SPI Connections
- SPI clock frequency depends on prescaler setting and system clock
- SS lines are active-low by default (check CF_SPI configuration)
- MISO line may need external pull-up/down depending on slave device

### GPIO
- Configure direction before use
- Internal pull-ups/downs depend on pad cell configuration
- Edge-detect works in input mode only

---

## Voltage Levels

All user project I/O operates at the voltage level set for the Caravel user area:
- Typical: 3.3V (vccd1/vccd2 domain)
- Ensure external devices are compatible with this voltage level
- Use level shifters if interfacing with devices at different voltages

---

## Example: Firmware Pad Configuration

```c
#include <firmware_apis.h>

void configure_user_pads(void) {
    // Configure SPI0 pads (mprj_io[11:5])
    ManagmentGpio_write(0);  // Start with mgmt_gpio low
    
    // SPI0 outputs: CLK, MOSI, SS[3:0]
    GPIOs_configure(5, GPIO_MODE_USER_STD_OUTPUT);   // SPI0_CLK
    GPIOs_configure(6, GPIO_MODE_USER_STD_OUTPUT);   // SPI0_MOSI
    GPIOs_configure(7, GPIO_MODE_USER_STD_INPUT_PULLDOWN);  // SPI0_MISO
    GPIOs_configure(8, GPIO_MODE_USER_STD_OUTPUT);   // SPI0_SS0
    GPIOs_configure(9, GPIO_MODE_USER_STD_OUTPUT);   // SPI0_SS1
    GPIOs_configure(10, GPIO_MODE_USER_STD_OUTPUT);  // SPI0_SS2
    GPIOs_configure(11, GPIO_MODE_USER_STD_OUTPUT);  // SPI0_SS3
    
    // SPI1 outputs: CLK, MOSI, SS[3:0]
    GPIOs_configure(12, GPIO_MODE_USER_STD_OUTPUT);  // SPI1_CLK
    GPIOs_configure(13, GPIO_MODE_USER_STD_OUTPUT);  // SPI1_MOSI
    GPIOs_configure(14, GPIO_MODE_USER_STD_INPUT_PULLDOWN);  // SPI1_MISO
    GPIOs_configure(15, GPIO_MODE_USER_STD_OUTPUT);  // SPI1_SS0
    GPIOs_configure(16, GPIO_MODE_USER_STD_OUTPUT);  // SPI1_SS1
    GPIOs_configure(17, GPIO_MODE_USER_STD_OUTPUT);  // SPI1_SS2
    GPIOs_configure(18, GPIO_MODE_USER_STD_OUTPUT);  // SPI1_SS3
    
    // I2C open-drain bidir
    GPIOs_configure(19, GPIO_MODE_USER_STD_BIDIRECTIONAL);  // I2C_SCL
    GPIOs_configure(20, GPIO_MODE_USER_STD_BIDIRECTIONAL);  // I2C_SDA
    GPIOs_configure(21, GPIO_MODE_USER_STD_OUTPUT);  // I2C_SCL_PU (optional)
    GPIOs_configure(22, GPIO_MODE_USER_STD_OUTPUT);  // I2C_SDA_PU (optional)
    
    // GPIO bidir
    GPIOs_configure(23, GPIO_MODE_USER_STD_BIDIRECTIONAL);  // GPIO_0
    GPIOs_configure(24, GPIO_MODE_USER_STD_BIDIRECTIONAL);  // GPIO_1
    
    // Unused as inputs
    for (int i = 25; i <= 37; i++) {
        GPIOs_configure(i, GPIO_MODE_USER_STD_INPUT_PULLDOWN);
    }
    
    // Load configuration
    GPIOs_loadConfigs();
    
    // Signal configuration complete
    ManagmentGpio_write(1);
}
```

---

*Refer to individual IP documentation for detailed signal timing and electrical characteristics.*
