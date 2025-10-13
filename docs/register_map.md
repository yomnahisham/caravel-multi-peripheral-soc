# Register Map

This document describes the register map for all peripherals integrated in the user project.

## Address Overview

| Peripheral | Base Address | Size  | Address Range            |
|-----------|--------------|-------|--------------------------|
| SPI0      | 0x3000_0000  | 64 KB | 0x3000_0000 - 0x3000_FFFF |
| SPI1      | 0x3001_0000  | 64 KB | 0x3001_0000 - 0x3001_FFFF |
| I2C       | 0x3002_0000  | 64 KB | 0x3002_0000 - 0x3002_FFFF |
| GPIO      | 0x3003_0000  | 64 KB | 0x3003_0000 - 0x3003_FFFF |

## Register Access Conventions

- **RW** - Read/Write
- **RO** - Read Only
- **WO** - Write Only
- **W1C** - Write 1 to Clear
- All registers are 32-bit word-aligned
- Byte-lane writes supported via Wishbone SEL signals
- Unused register bits read as 0

---

## SPI0 / SPI1 Registers (CF_SPI)

Both SPI controllers are identical instances of the CF_SPI IP core.

### Base Addresses
- SPI0: 0x3000_0000
- SPI1: 0x3001_0000

### Register Map

Refer to the CF_SPI IP documentation at `/nc/ip/CF_SPI/` for complete register details.

#### Key Registers (relative offsets from base)

| Offset | Name       | Access | Reset | Description |
|--------|-----------|--------|-------|-------------|
| 0x00   | RXDATA    | RO     | 0x00  | Receive Data Register |
| 0x04   | TXDATA    | WO     | -     | Transmit Data Register |
| 0x08   | STATUS    | RO     | 0x00  | Status Register |
| 0x0C   | CTRL      | RW     | 0x00  | Control Register |
| 0x10   | CFG       | RW     | 0x00  | Configuration Register |
| 0x14   | PRESCALER | RW     | 0x00  | Clock Prescaler |
| 0x18   | IM        | RW     | 0x00  | Interrupt Mask |
| 0x1C   | MIS       | RO     | 0x00  | Masked Interrupt Status |
| 0x20   | RIS       | RO     | 0x00  | Raw Interrupt Status |
| 0x24   | IC        | W1C    | -     | Interrupt Clear |

#### STATUS Register (Offset 0x08)

| Bits  | Name    | Access | Reset | Description |
|-------|---------|--------|-------|-------------|
| [0]   | BUSY    | RO     | 0     | SPI Busy Flag (1 = transfer in progress) |
| [1]   | TFE     | RO     | 1     | Transmit FIFO Empty |
| [2]   | TFNF    | RO     | 1     | Transmit FIFO Not Full |
| [3]   | RFNE    | RO     | 0     | Receive FIFO Not Empty |
| [4]   | RFF     | RO     | 0     | Receive FIFO Full |
| [31:5]| -       | -      | 0     | Reserved |

#### CTRL Register (Offset 0x0C)

| Bits  | Name    | Access | Reset | Description |
|-------|---------|--------|-------|-------------|
| [0]   | ENABLE  | RW     | 0     | SPI Enable (1 = enabled) |
| [1]   | CPOL    | RW     | 0     | Clock Polarity |
| [2]   | CPHA    | RW     | 0     | Clock Phase |
| [3]   | LSBFIRST| RW     | 0     | LSB First (1 = LSB first, 0 = MSB first) |
| [7:4] | SSMODE  | RW     | 0     | Slave Select Mode |
| [31:8]| -       | -      | 0     | Reserved |

---

## I2C Registers (EF_I2C)

### Base Address
- I2C: 0x3002_0000

### Register Map

Refer to the EF_I2C IP documentation at `/nc/ip/EF_I2C/` for complete register details.

#### Key Registers (relative offsets from base)

| Offset | Name       | Access | Reset | Description |
|--------|-----------|--------|-------|-------------|
| 0x00   | PRESCALER | RW     | 0x00  | Clock Prescaler Register (Low byte) |
| 0x04   | PRESCALERH| RW     | 0x00  | Clock Prescaler Register (High byte) |
| 0x08   | CTRL      | RW     | 0x00  | Control Register |
| 0x0C   | TXR       | WO     | -     | Transmit Register |
| 0x0C   | RXR       | RO     | 0x00  | Receive Register (same address as TXR) |
| 0x10   | CMD       | WO     | -     | Command Register |
| 0x10   | STATUS    | RO     | 0x00  | Status Register (same address as CMD) |
| 0x14   | IM        | RW     | 0x00  | Interrupt Mask |
| 0x18   | MIS       | RO     | 0x00  | Masked Interrupt Status |
| 0x1C   | RIS       | RO     | 0x00  | Raw Interrupt Status |
| 0x20   | IC        | W1C    | -     | Interrupt Clear |

#### CTRL Register (Offset 0x08)

| Bits  | Name    | Access | Reset | Description |
|-------|---------|--------|-------|-------------|
| [0]   | -       | -      | 0     | Reserved |
| [6]   | EN      | RW     | 0     | I2C Enable |
| [7]   | IEN     | RW     | 0     | Interrupt Enable |
| [31:8]| -       | -      | 0     | Reserved |

#### CMD Register (Offset 0x10, Write Only)

| Bits  | Name    | Access | Reset | Description |
|-------|---------|--------|-------|-------------|
| [0]   | -       | -      | 0     | Reserved |
| [3]   | STO     | WO     | -     | Generate STOP condition |
| [4]   | RD      | WO     | -     | Read from slave |
| [5]   | WR      | WO     | -     | Write to slave |
| [6]   | ACK     | WO     | -     | Send ACK (1 = NACK, 0 = ACK) |
| [7]   | STA     | WO     | -     | Generate START condition |
| [31:8]| -       | -      | 0     | Reserved |

#### STATUS Register (Offset 0x10, Read Only)

| Bits  | Name    | Access | Reset | Description |
|-------|---------|--------|-------|-------------|
| [0]   | IF      | RO     | 0     | Interrupt Flag |
| [1]   | TIP     | RO     | 0     | Transfer in Progress |
| [5]   | AL      | RO     | 0     | Arbitration Lost |
| [6]   | BUSY    | RO     | 0     | I2C Busy |
| [7]   | RXACK   | RO     | 0     | Received Acknowledge from slave |
| [31:8]| -       | -      | 0     | Reserved |

---

## GPIO Registers (EF_GPIO8)

The EF_GPIO8 IP provides 8 GPIO lines. For this project, we utilize 2 lines with edge-detect interrupt capability.

### Base Address
- GPIO: 0x3003_0000

### Register Map

Refer to the EF_GPIO8 IP documentation at `/nc/ip/EF_GPIO8/` for complete register details.

#### Key Registers (relative offsets from base)

| Offset | Name       | Access | Reset | Description |
|--------|-----------|--------|-------|-------------|
| 0x00   | DATA      | RW     | 0x00  | GPIO Data Register |
| 0x04   | DIR       | RW     | 0x00  | Direction Register (0=in, 1=out) |
| 0x08   | IM        | RW     | 0x00  | Interrupt Mask Register |
| 0x0C   | MIS       | RO     | 0x00  | Masked Interrupt Status |
| 0x10   | RIS       | RO     | 0x00  | Raw Interrupt Status |
| 0x14   | IC        | W1C    | -     | Interrupt Clear |
| 0x18   | EDGE      | RW     | 0x00  | Edge Select (0=level, 1=edge) |
| 0x1C   | BOTH      | RW     | 0x00  | Both Edges (0=single, 1=both) |
| 0x20   | RISE      | RW     | 0x00  | Rising Edge Enable |
| 0x24   | FALL      | RW     | 0x00  | Falling Edge Enable |

#### DATA Register (Offset 0x00)

| Bits  | Name     | Access | Reset | Description |
|-------|----------|--------|-------|-------------|
| [7:0] | DATA[7:0]| RW     | 0x00  | GPIO Pin Data (bit 0-1 used in this project) |
| [31:8]| -        | -      | 0     | Reserved |

#### DIR Register (Offset 0x04)

| Bits  | Name     | Access | Reset | Description |
|-------|----------|--------|-------|-------------|
| [7:0] | DIR[7:0] | RW     | 0x00  | Direction (0 = input, 1 = output) |
| [31:8]| -        | -      | 0     | Reserved |

#### IM Register (Offset 0x08)

| Bits  | Name     | Access | Reset | Description |
|-------|----------|--------|-------|-------------|
| [7:0] | IM[7:0]  | RW     | 0x00  | Interrupt Mask (1 = enabled) |
| [31:8]| -        | -      | 0     | Reserved |

#### EDGE Register (Offset 0x18)

| Bits  | Name      | Access | Reset | Description |
|-------|-----------|--------|-------|-------------|
| [7:0] | EDGE[7:0] | RW     | 0x00  | Edge detection mode (0 = level, 1 = edge) |
| [31:8]| -         | -      | 0     | Reserved |

#### RISE Register (Offset 0x20)

| Bits  | Name      | Access | Reset | Description |
|-------|-----------|--------|-------|-------------|
| [7:0] | RISE[7:0] | RW     | 0x00  | Rising edge enable (1 = detect rising edge) |
| [31:8]| -         | -      | 0     | Reserved |

#### FALL Register (Offset 0x24)

| Bits  | Name      | Access | Reset | Description |
|-------|-----------|--------|-------|-------------|
| [7:0] | FALL[7:0] | RW     | 0x00  | Falling edge enable (1 = detect falling edge) |
| [31:8]| -         | -      | 0     | Reserved |

---

## Interrupt Sources

The user project maps peripheral interrupts to Caravel's `user_irq[2:0]` signals:

| Signal       | Source         | Description |
|-------------|----------------|-------------|
| user_irq[0] | SPI0 interrupt | SPI0 transfer complete / error |
| user_irq[1] | SPI1 interrupt | SPI1 transfer complete / error |
| user_irq[2] | I2C / GPIO interrupt | I2C events OR GPIO edge-detect events |

**Note:** Since Caravel provides only 3 interrupt lines, the I2C and GPIO interrupts are OR'd together on `user_irq[2]`. Software must poll both peripheral interrupt status registers to determine the source.

---

## Programming Examples

### Example 1: SPI Master Write

```c
// Configure SPI0
*(volatile uint32_t*)(0x30000010) = 0x04;  // Set prescaler
*(volatile uint32_t*)(0x3000000C) = 0x01;  // Enable SPI

// Write data
while (!(*(volatile uint32_t*)(0x30000008) & 0x02));  // Wait for TFNF
*(volatile uint32_t*)(0x30000004) = 0xAA;  // Write data to TXDATA

// Wait for completion
while (*(volatile uint32_t*)(0x30000008) & 0x01);  // Wait while BUSY
```

### Example 2: I2C Master Write

```c
// Configure I2C
*(volatile uint32_t*)(0x30020000) = 0x32;  // Set prescaler low
*(volatile uint32_t*)(0x30020008) = 0x80;  // Enable I2C with interrupts

// Start condition + address
*(volatile uint32_t*)(0x3002000C) = 0xA0;  // Slave address (write)
*(volatile uint32_t*)(0x30020010) = 0x90;  // STA + WR

// Wait and write data
while (*(volatile uint32_t*)(0x30020010) & 0x02);  // Wait while TIP
*(volatile uint32_t*)(0x3002000C) = 0x55;  // Data byte
*(volatile uint32_t*)(0x30020010) = 0x10;  // WR

// Stop condition
*(volatile uint32_t*)(0x30020010) = 0x50;  // STO + WR
```

### Example 3: GPIO Edge Detection

```c
// Configure GPIO pins 0-1 as inputs with rising edge detection
*(volatile uint32_t*)(0x30030004) = 0x00;  // DIR: both inputs
*(volatile uint32_t*)(0x30030018) = 0x03;  // EDGE: edge mode for pins 0-1
*(volatile uint32_t*)(0x30030020) = 0x03;  // RISE: rising edge for pins 0-1
*(volatile uint32_t*)(0x30030008) = 0x03;  // IM: unmask interrupts for pins 0-1

// In interrupt handler:
uint32_t status = *(volatile uint32_t*)(0x3003000C);  // Read MIS
*(volatile uint32_t*)(0x30030014) = status;  // Clear interrupts (W1C)
```

---

## Notes

1. All IP cores follow the Wishbone B4 classic protocol
2. Peripheral internal registers may have additional fields - refer to IP documentation
3. Byte-lane writes via Wishbone SEL signals are supported
4. Invalid addresses return 0xDEADBEEF on reads and are ACK'd but discarded on writes
5. For full register specifications, consult the individual IP documentation in `/nc/ip/`

