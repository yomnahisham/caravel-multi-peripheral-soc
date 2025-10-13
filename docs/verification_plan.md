# Caravel Multi-Peripheral User Project Verification Plan

## Overview
This document outlines the verification strategy for the Caravel user project integrating 2× SPI masters, 1× I2C controller, and 2× GPIO lines with edge-detect interrupts.

## Design Under Test (DUT)
- **Module**: user_project_wrapper (top-level)
- **Core Module**: user_project (Wishbone slave with address decoder)
- **Peripherals**:
  - CF_SPI_WB × 2 (SPI0 at 0x3000_0000, SPI1 at 0x3001_0000)
  - EF_I2C_WB × 1 (I2C at 0x3002_0000)
  - EF_GPIO8_WB × 1 (GPIO at 0x3003_0000, using only 2 GPIO pins)

## Clock Configuration
- **Clock Frequency**: 40 MHz
- **Clock Period**: 25 ns
- **Source**: Caravel wb_clk_i

## Test Strategy

### Test Suite Organization

#### Test 1: basic_test
**Purpose**: Validate firmware execution and basic connectivity
**Pass Criteria**: Firmware executes successfully, management GPIO handshake works

#### Test 2: spi0_test
**Purpose**: Verify SPI0 peripheral integration and Wishbone access
**Pass Criteria**: SPI0 registers accessible via Wishbone (0x3000_0000), basic SPI operation

#### Test 3: spi1_test
**Purpose**: Verify SPI1 peripheral integration (independent from SPI0)
**Pass Criteria**: SPI1 registers accessible via Wishbone (0x3001_0000), address decoding works

#### Test 4: i2c_test
**Purpose**: Verify I2C controller integration
**Pass Criteria**: I2C registers accessible via Wishbone (0x3002_0000), open-drain pins work

#### Test 5: gpio_test
**Purpose**: Verify GPIO with edge-detect interrupt functionality
**Pass Criteria**: GPIO registers accessible, direction control works, interrupts trigger

#### Test 6: system_integration_test
**Purpose**: Comprehensive integration test exercising all peripherals
**Pass Criteria**: All peripherals accessible, no conflicts, interrupts routed correctly

## Success Criteria
1. All 6 tests pass without errors
2. No simulation timeouts (within 600s per test)
3. All Wishbone transactions acknowledged within 1 cycle
4. All interrupts properly routed and observable

---
**Document Version**: 1.0  
**Date**: 2025-10-10  
**Status**: Initial Plan
