from caravel_cocotb.caravel_interfaces import test_configure, report_test
import cocotb

@cocotb.test()
@report_test
async def basic_test(dut):
    caravelEnv = await test_configure(dut, timeout_cycles=1000000)
    
    await caravelEnv.release_csb()
    
    cocotb.log.info("[TEST] Waiting for firmware configuration...")
    await caravelEnv.wait_mgmt_gpio(1)
    
    cocotb.log.info("[TEST] Firmware configuration complete")
    cocotb.log.info("[TEST] PASS - Basic test completed successfully")
