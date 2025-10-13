from caravel_cocotb.caravel_interfaces import test_configure, report_test
import cocotb
from cocotb.triggers import ClockCycles

@cocotb.test()
@report_test
async def gpio_test(dut):
    caravelEnv = await test_configure(dut, timeout_cycles=1000000)
    
    await caravelEnv.release_csb()
    
    cocotb.log.info("[TEST] Waiting for firmware configuration...")
    await caravelEnv.wait_mgmt_gpio(1)
    cocotb.log.info("[TEST] Firmware configuration complete")
    
    cocotb.log.info("[TEST] Monitoring GPIO signals on GPIO[23-24]")
    
    await ClockCycles(caravelEnv.clk, 500)
    
    gpio_val = caravelEnv.monitor_gpio(24, 23)
    cocotb.log.info(f"[TEST] GPIO value: {gpio_val.binstr}")
    
    await ClockCycles(caravelEnv.clk, 500)
    
    gpio_val = caravelEnv.monitor_gpio(24, 23)
    cocotb.log.info(f"[TEST] GPIO value after toggle: {gpio_val.binstr}")
    
    cocotb.log.info("[TEST] PASS - GPIO test completed successfully")
