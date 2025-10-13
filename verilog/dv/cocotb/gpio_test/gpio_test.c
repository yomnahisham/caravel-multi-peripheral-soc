#include <firmware_apis.h>

#define GPIO_BASE 0x30030000

#define GPIO_DATAI_OFFSET 0
#define GPIO_DATAO_OFFSET 1
#define GPIO_DIR_OFFSET 2
#define GPIO_IM_OFFSET 16320
#define GPIO_GCLK_OFFSET 16324

void main() {
    ManagmentGpio_outputEnable();
    ManagmentGpio_write(0);
    
    enableHkSpi(0);
    
    GPIOs_configure(23, GPIO_MODE_USER_STD_BIDIRECTIONAL);
    GPIOs_configure(24, GPIO_MODE_USER_STD_BIDIRECTIONAL);
    
    GPIOs_loadConfigs();
    
    User_enableIF();
    
    ManagmentGpio_write(1);
    
    USER_writeWord(0x00000001, GPIO_GCLK_OFFSET);
    
    USER_writeWord(0x00000001, GPIO_DIR_OFFSET);
    
    USER_writeWord(0x00000001, GPIO_DATAO_OFFSET);
    
    USER_writeWord(0x00000003, GPIO_IM_OFFSET);
    
    dummyDelay(100);
    
    USER_writeWord(0x00000000, GPIO_DATAO_OFFSET);
    
    dummyDelay(100);
    
    int datai = USER_readWord(GPIO_DATAI_OFFSET);
    
    return;
}
