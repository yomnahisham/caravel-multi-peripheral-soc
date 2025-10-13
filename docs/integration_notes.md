# Integration Notes

This document provides detailed integration information for the Caravel user project.

## Clock and Reset Strategy

### Clock Domain

**Single Clock Domain Design:**
- All peripherals operate on `wb_clk_i` from the Caravel Wishbone bus
- No clock domain crossing logic required
- Typical frequency: 25 MHz (configurable via Caravel PLL)
- No derived or gated clocks used

### Reset Strategy

**Synchronous Active-High Reset:**
- Reset signal: `wb_rst_i` from Caravel Wishbone bus
- Type: Synchronous, active-high
- All peripheral IP cores support synchronous reset
- Reset is asserted during Caravel power-on and system reset
- All registers initialize to defined reset values

**Reset Sequence:**
1. Caravel asserts `wb_rst_i` (high) during startup
2. All peripherals reset their internal state
3. `wb_rst_i` deasserted (low) - normal operation begins
4. Software can access peripherals via Wishbone bus

---

## Wishbone Bus Architecture

### Wishbone B4 Classic Protocol

The user project implements a Wishbone B4 compliant slave interface:

**Key Signals:**
- `wb_clk_i` - Bus clock (input)
- `wb_rst_i` - Synchronous reset, active high (input)
- `wbs_cyc_i` - Bus cycle indicator (input)
- `wbs_stb_i` - Address/data strobe (input)
- `wbs_we_i` - Write enable (1=write, 0=read) (input)
- `wbs_sel_i[3:0]` - Byte lane selects (input)
- `wbs_adr_i[31:0]` - Address bus (input)
- `wbs_dat_i[31:0]` - Write data bus (input)
- `wbs_dat_o[31:0]` - Read data bus (output)
- `wbs_ack_o` - Transfer acknowledge (output)

### Address Decoding

The user_project implements address decoding based on bits [19:16] of the address bus:

```verilog
// Address decode for 4 peripherals (64KB windows)
wire [1:0] periph_sel = wbs_adr_i[19:16];

// Peripheral select strobes (gated STB)
wire spi0_stb = (periph_sel == 2'b00) & wbs_stb_i;
wire spi1_stb = (periph_sel == 2'b01) & wbs_stb_i;
wire i2c_stb  = (periph_sel == 2'b10) & wbs_stb_i;
wire gpio_stb = (periph_sel == 2'b11) & wbs_stb_i;
```

**Critical Rule:** `wbs_cyc_i` is NEVER gated or qualified with address decode. It is routed directly to all peripherals. Only `wbs_stb_i` is gated for peripheral selection.

### Bus Timing

**Standard Read Cycle:**
```
Clock:     __/‾‾\__/‾‾\__/‾‾\__
cyc_i:     ____/‾‾‾‾‾‾‾‾‾‾\____
stb_i:     ____/‾‾‾‾‾‾‾\________
ack_o:     ________/‾‾‾\________
dat_o:     ========<VALID>======
           |   |   |   |
           T0  T1  T2  T3
```
- T0: Master asserts CYC, STB, address
- T1: Slave decodes address, fetches data
- T2: Slave asserts ACK, drives valid data
- T3: Master samples data, deasserts CYC/STB

**Standard Write Cycle:**
```
Clock:     __/‾‾\__/‾‾\__/‾‾\__
cyc_i:     ____/‾‾‾‾‾‾‾‾‾‾\____
stb_i:     ____/‾‾‾‾‾‾‾\________
we_i:      ____/‾‾‾‾‾‾‾\________
ack_o:     ________/‾‾‾\________
dat_i:     ====<VALID>==========
           |   |   |   |
           T0  T1  T2  T3
```
- T0: Master asserts CYC, STB, WE, address, data
- T1: Slave decodes address
- T2: Slave writes data, asserts ACK
- T3: Master deasserts CYC/STB

**Latency:** One-cycle read/write latency (ACK on cycle after STB)

### Response Multiplexing

Read data from selected peripheral is multiplexed:

```verilog
always @(*) begin
    case (periph_sel)
        2'b00: wbs_dat_o = spi0_rdata;
        2'b01: wbs_dat_o = spi1_rdata;
        2'b10: wbs_dat_o = i2c_rdata;
        2'b11: wbs_dat_o = gpio_rdata;
        default: wbs_dat_o = 32'hDEADBEEF;  // Invalid address
    endcase
end

assign wbs_ack_o = spi0_ack | spi1_ack | i2c_ack | gpio_ack;
```

### Error Handling

- **Invalid Address Reads:** Return `0xDEADBEEF`
- **Invalid Address Writes:** Assert ACK but discard data
- **No ACK Timeout:** If no peripheral ACKs, bus will hang (design should ensure all valid addresses are ACK'd)

---

## Interrupt Architecture

### Interrupt Mapping

The user project maps peripheral interrupts to Caravel's three user interrupt lines:

| Caravel Signal | Source | Trigger |
|---------------|--------|---------|
| `user_irq[0]` | SPI0 | Transfer complete, FIFO events, errors |
| `user_irq[1]` | SPI1 | Transfer complete, FIFO events, errors |
| `user_irq[2]` | I2C OR GPIO | I2C events OR GPIO edge-detect |

### Interrupt Connection

```verilog
// Direct connection (level-sensitive, active-high)
assign user_irq[0] = spi0_irq;
assign user_irq[1] = spi1_irq;
assign user_irq[2] = i2c_irq | gpio_irq;
```

### Interrupt Handling

**Firmware Interrupt Handler:**
```c
void irq_handler(void) {
    uint32_t irq_status = read_csr(mip);  // RISC-V interrupt pending
    
    if (irq_status & USER_IRQ_0_MASK) {
        // SPI0 interrupt
        uint32_t spi0_status = *(volatile uint32_t*)(0x30000020);  // RIS
        // Handle SPI0 interrupt
        *(volatile uint32_t*)(0x30000024) = spi0_status;  // Clear (IC)
    }
    
    if (irq_status & USER_IRQ_1_MASK) {
        // SPI1 interrupt
        uint32_t spi1_status = *(volatile uint32_t*)(0x30010020);  // RIS
        // Handle SPI1 interrupt
        *(volatile uint32_t*)(0x30010024) = spi1_status;  // Clear (IC)
    }
    
    if (irq_status & USER_IRQ_2_MASK) {
        // Check both I2C and GPIO
        uint32_t i2c_status = *(volatile uint32_t*)(0x3002001C);  // RIS
        uint32_t gpio_status = *(volatile uint32_t*)(0x30030010);  // RIS
        
        if (i2c_status) {
            // Handle I2C interrupt
            *(volatile uint32_t*)(0x30020020) = i2c_status;  // Clear
        }
        
        if (gpio_status) {
            // Handle GPIO interrupt
            *(volatile uint32_t*)(0x30030014) = gpio_status;  // Clear
        }
    }
}
```

### Interrupt Enable/Mask

Each peripheral has its own interrupt mask register:
- SPI0/SPI1: IM register (offset 0x18)
- I2C: IM register (offset 0x14)
- GPIO: IM register (offset 0x08)

Global interrupt enable is controlled by the RISC-V core's interrupt mask.

---

## Power Connections

### Power Domains

**User Project Power:**
- `vccd1` - Digital 1.8V power (primary)
- `vssd1` - Digital ground (primary)
- `vccd2` - Digital 1.8V power (secondary, typically same as vccd1)
- `vssd2` - Digital ground (secondary)

**This Design:**
All logic connects to `vccd1/vssd1` power domain.

### Power Pins in Verilog

```verilog
`ifdef USE_POWER_PINS
    inout vccd1,    // User area 1 1.8V power
    inout vssd1,    // User area 1 digital ground
`endif
```

Power pins are connected through the hierarchy:
- `user_project_wrapper` → `user_project` → peripheral instances

---

## Peripheral-Specific Integration Notes

### SPI Master (CF_SPI)

**Features:**
- Configurable clock polarity and phase (CPOL, CPHA)
- Multiple slave select lines (up to 4)
- Programmable clock prescaler
- FIFO for transmit and receive
- Interrupt on transfer complete, FIFO thresholds

**Integration:**
- Clock source: `wb_clk_i`
- Reset: `wb_rst_i`
- Two independent instances (SPI0, SPI1)
- Each instance has 7 I/O signals (CLK, MOSI, MISO, SS[3:0])

**Timing:**
- SPI clock frequency = wb_clk_i / (2 × (PRESCALER + 1))
- For 25 MHz wb_clk: PRESCALER=0 → 12.5 MHz, PRESCALER=1 → 6.25 MHz

### I2C Controller (EF_I2C)

**Features:**
- Master mode operation
- Standard mode (100 kHz) and Fast mode (400 kHz)
- Multi-master arbitration
- Clock stretching support
- Programmable prescaler

**Integration:**
- Clock source: `wb_clk_i`
- Reset: `wb_rst_i`
- 2 bidirectional I/O signals (SCL, SDA) - requires open-drain
- Optional pull-up control signals

**Open-Drain Configuration:**
```verilog
// For SCL (similar for SDA)
assign io_out[19] = 1'b0;           // Always 0
assign io_oeb[19] = scl_oen;        // 0=drive low, 1=high-Z
assign scl_in = io_in[19];          // Read line state
```

**Timing:**
- I2C SCL frequency = wb_clk_i / (5 × PRESCALER)
- For 25 MHz wb_clk, 100 kHz I2C: PRESCALER = 50
- For 25 MHz wb_clk, 400 kHz I2C: PRESCALER = 12

### GPIO with Edge Detection (EF_GPIO8)

**Features:**
- 8-bit GPIO (using 2 bits in this design)
- Configurable direction per pin
- Edge-detect interrupts (rising, falling, both)
- Level-sensitive interrupts
- Individual interrupt mask per pin

**Integration:**
- Clock source: `wb_clk_i`
- Reset: `wb_rst_i`
- Using pins [1:0] for this project
- Each pin is bidirectional

**Edge Detection Configuration:**
- EDGE register: Select edge (1) or level (0) mode
- RISE register: Enable rising edge detection
- FALL register: Enable falling edge detection
- BOTH register: Detect both edges (overrides RISE/FALL)

---

## Simulation and Verification

### Cocotb Test Environment

**Setup:**
1. Run setup script: `python verilog/dv/setup-cocotb.py /workspace`
2. Add RTL files to `verilog/includes/includes.rtl.caravel_user_project`
3. Tests located in `verilog/dv/cocotb/<test_name>/`

**Test Structure:**
Each test consists of:
- `<test_name>.py` - Python cocotb test
- `<test_name>.c` - Firmware running on management core
- `<test_name>.yaml` - Test configuration (optional)

**Running Tests:**
```bash
cd verilog/dv/cocotb
caravel_cocotb -t <test_name> -tag <run_tag>           # RTL
caravel_cocotb -t <test_name> -tag <run_tag> -sim GL   # Gate-level
```

### Synchronization Pattern

**Firmware:**
```c
// Configure GPIOs, peripherals, etc.
ManagmentGpio_write(0);
// ... configuration code ...
ManagmentGpio_write(1);  // Signal ready to testbench
// ... test code ...
```

**Python Testbench:**
```python
@cocotb.test()
@report_test
async def test_name(dut):
    caravelEnv = await test_configure(dut, timeout_cycles=10_000_000)
    await caravelEnv.release_csb()
    await caravelEnv.wait_mgmt_gpio(1)  # Wait for firmware ready
    # ... test stimulus and checking ...
```

### Test Coverage

Required tests:
1. **spi0_test** - SPI0 master functionality
2. **spi1_test** - SPI1 master functionality
3. **i2c_test** - I2C master transactions
4. **gpio_test** - GPIO edge-detect interrupts
5. **system_integration_test** - All peripherals together

---

## Physical Design Notes

### Floorplanning

**user_project Macro:**
- Estimated area: ~300 µm × 300 µm
- Die area in config: 400 µm × 400 µm (minimum for pin placement)
- Contains: address decode logic + 4 peripheral instances

**user_project_wrapper:**
- Fixed size per Caravel spec: ~2900 µm × 3500 µm
- Contains: user_project macro + pad connections

### Pin Order

Pin order configuration file: `openlane/user_project/pin_order.cfg`

Uses standard Caravel pin ordering:
- Wishbone bus signals (grouped)
- Logic analyzer signals (grouped)
- IRQ signals (grouped)
- I/O pads (sequential by number)

### OpenLane Configuration

**Key Settings:**
```json
{
    "DESIGN_NAME": "user_project",
    "CLOCK_PORT": "wb_clk_i",
    "CLOCK_PERIOD": 25,
    "DIE_AREA": "0 0 400 400",
    "FP_PIN_ORDER_CFG": "dir::pin_order.cfg"
}
```

---

## Timing Constraints

### Clock Constraints

Primary clock: `wb_clk_i` at 25 MHz (40 ns period)

```tcl
create_clock -name wb_clk -period 40 [get_ports wb_clk_i]
```

### Input/Output Delays

```tcl
set_input_delay -clock wb_clk -max 10 [all_inputs]
set_input_delay -clock wb_clk -min 2 [all_inputs]
set_output_delay -clock wb_clk -max 10 [all_outputs]
set_output_delay -clock wb_clk -min 2 [all_outputs]
```

### False Paths

No false paths in this single clock domain design.

---

## Known Limitations and Constraints

1. **Interrupt Sharing:** I2C and GPIO share user_irq[2] - software must poll both
2. **I2C Pull-ups:** Require external pull-up resistors (not on-chip)
3. **SPI Modes:** CF_SPI supports standard SPI modes (check IP docs for details)
4. **GPIO Count:** EF_GPIO8 has 8 bits but only 2 are used in this design
5. **Address Windows:** 64KB per peripheral (larger than minimum required)
6. **No Clock Gating:** Design uses single ungated clock (simplicity over power)

---

## Debug and Troubleshooting

### Common Issues

**Issue: No ACK from peripheral**
- Check address decode logic
- Verify peripheral select strobe
- Ensure `wbs_cyc_i` is not gated
- Confirm peripheral is out of reset

**Issue: Incorrect read data**
- Check read data multiplexer
- Verify peripheral address decode
- Check for bus timing violations

**Issue: Interrupts not working**
- Verify interrupt mask registers are set
- Check interrupt wiring in user_project
- Confirm RISC-V core interrupt enable
- Verify interrupt clear mechanism (W1C)

**Issue: I2C not communicating**
- Verify open-drain configuration
- Check external pull-ups present
- Verify prescaler for correct clock frequency
- Check START/STOP conditions

**Issue: SPI clock frequency wrong**
- Check prescaler value
- Verify wb_clk_i frequency
- Calculate: SPI_CLK = wb_clk_i / (2 × (PRESCALER + 1))

### Debugging Tools

**Waveform Viewing:**
```bash
# Waves are in sim/<tag>/<simtype>-<testname>/waves.vcd
gtkwave sim/<tag>/rtl-<testname>/waves.vcd
```

**Firmware Debug:**
- UART output via Caravel's UART interface
- Management GPIO for simple signaling
- Logic analyzer lines (la_data_in/out) for observability

**Register Dumps:**
```c
// Read and print all registers of a peripheral
for (int i = 0; i < 16; i++) {
    uint32_t val = *(volatile uint32_t*)(BASE_ADDR + i*4);
    print_hex(i*4, val);
}
```

---

## References and Resources

### IP Documentation
- CF_SPI: `/nc/ip/CF_SPI/docs/`
- EF_I2C: `/nc/ip/EF_I2C/docs/`
- EF_GPIO8: `/nc/ip/EF_GPIO8/docs/`

### Caravel Resources
- Caravel Documentation: https://caravel-harness.readthedocs.io/
- Caravel GitHub: https://github.com/efabless/caravel
- Caravel User Project: https://github.com/efabless/caravel_user_project

### OpenLane
- OpenLane Documentation: https://openlane2.readthedocs.io/
- OpenLane GitHub: https://github.com/efabless/openlane2

### Design Specifications
- Wishbone B4 Specification: https://cdn.opencores.org/downloads/wbspec_b4.pdf

---

*Document Version: 1.0*  
*Last Updated: 2025-10-10*
