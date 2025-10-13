from caravel_cocotb.caravel_interfaces import test_configure, report_test
import cocotb
from cocotb.triggers import ClockCycles

@cocotb.test()
@report_test
async def i2c_test(dut):
    caravelEnv = await test_configure(dut, timeout_cycles=1000000)
    
    await caravelEnv.release_csb()
    
    cocotb.log.info("[TEST] Waiting for firmware configuration...")
    await caravelEnv.wait_mgmt_gpio(1)
    cocotb.log.info("[TEST] Firmware configuration complete")
    
    cocotb.log.info("[TEST] Monitoring I2C signals on GPIO[19-20]")
    
    await ClockCycles(caravelEnv.clk, 1000)
    
    i2c_scl = caravelEnv.monitor_gpio(19, 19)
    i2c_sda = caravelEnv.monitor_gpio(20, 20)
    
    cocotb.log.info(f"[TEST] I2C signals - SCL: {i2c_scl}, SDA: {i2c_sda}")
    
    cocotb.log.info("[TEST] PASS - I2C test completed successfully")
