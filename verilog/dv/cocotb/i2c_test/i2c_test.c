#include <firmware_apis.h>

#define I2C_BASE 0x30020000
#define I2C_PRESCALER_OFFSET 0
#define I2C_CTRL_OFFSET 4
#define I2C_STATUS_OFFSET 8

void main() {
    ManagmentGpio_outputEnable();
    ManagmentGpio_write(0);
    
    enableHkSpi(0);
    
    GPIOs_configure(19, GPIO_MODE_USER_STD_BIDIRECTIONAL);
    GPIOs_configure(20, GPIO_MODE_USER_STD_BIDIRECTIONAL);
    
    GPIOs_loadConfigs();
    
    User_enableIF();
    
    ManagmentGpio_write(1);
    
    USER_writeWord(0x00000001, 1);
    
    USER_writeWord(100, 0);
    
    USER_writeWord(0x00000001, 1);
    
    int status = USER_readWord(2);
    
    return;
}
