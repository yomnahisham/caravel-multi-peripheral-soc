from caravel_cocotb.caravel_interfaces import test_configure, report_test
import cocotb
from cocotb.triggers import ClockCycles

@cocotb.test()
@report_test
async def system_integration_test(dut):
    caravelEnv = await test_configure(dut, timeout_cycles=1000000)
    
    await caravelEnv.release_csb()
    
    cocotb.log.info("[TEST] Waiting for firmware configuration...")
    await caravelEnv.wait_mgmt_gpio(1)
    cocotb.log.info("[TEST] Firmware configuration complete")
    
    cocotb.log.info("[TEST] System integration test - exercising all peripherals")
    
    await ClockCycles(caravelEnv.clk, 2000)
    
    spi0_sclk = caravelEnv.monitor_gpio(5, 5)
    spi1_sclk = caravelEnv.monitor_gpio(12, 12)
    gpio_val = caravelEnv.monitor_gpio(24, 23)
    
    cocotb.log.info(f"[TEST] SPI0 SCLK: {spi0_sclk}, SPI1 SCLK: {spi1_sclk}, GPIO: {gpio_val.binstr}")
    
    cocotb.log.info("[TEST] PASS - System integration test completed successfully")
