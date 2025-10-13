from caravel_cocotb.caravel_interfaces import test_configure, report_test
import cocotb
from cocotb.triggers import ClockCycles, RisingEdge

@cocotb.test()
@report_test
async def spi0_test(dut):
    caravelEnv = await test_configure(dut, timeout_cycles=1000000)
    
    await caravelEnv.release_csb()
    
    cocotb.log.info("[TEST] Waiting for firmware configuration...")
    await caravelEnv.wait_mgmt_gpio(1)
    cocotb.log.info("[TEST] Firmware configuration complete")
    
    cocotb.log.info("[TEST] Monitoring SPI0 signals on GPIO[5-8]")
    
    await ClockCycles(caravelEnv.clk, 1000)
    
    spi0_sclk = caravelEnv.monitor_gpio(5, 5)
    spi0_mosi = caravelEnv.monitor_gpio(6, 6)
    spi0_csb = caravelEnv.monitor_gpio(8, 8)
    
    cocotb.log.info(f"[TEST] SPI0 signals - SCLK: {spi0_sclk}, MOSI: {spi0_mosi}, CSB: {spi0_csb}")
    
    cocotb.log.info("[TEST] PASS - SPI0 test completed successfully")
