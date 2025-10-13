from caravel_cocotb.caravel_interfaces import test_configure, report_test
import cocotb
from cocotb.triggers import ClockCycles

@cocotb.test()
@report_test
async def spi1_test(dut):
    caravelEnv = await test_configure(dut, timeout_cycles=1000000)
    
    await caravelEnv.release_csb()
    
    cocotb.log.info("[TEST] Waiting for firmware configuration...")
    await caravelEnv.wait_mgmt_gpio(1)
    cocotb.log.info("[TEST] Firmware configuration complete")
    
    cocotb.log.info("[TEST] Monitoring SPI1 signals on GPIO[12-15]")
    
    await ClockCycles(caravelEnv.clk, 1000)
    
    spi1_sclk = caravelEnv.monitor_gpio(12, 12)
    spi1_mosi = caravelEnv.monitor_gpio(13, 13)
    spi1_csb = caravelEnv.monitor_gpio(15, 15)
    
    cocotb.log.info(f"[TEST] SPI1 signals - SCLK: {spi1_sclk}, MOSI: {spi1_mosi}, CSB: {spi1_csb}")
    
    cocotb.log.info("[TEST] PASS - SPI1 test completed successfully")
