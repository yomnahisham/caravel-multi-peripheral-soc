#include <firmware_apis.h>

void main() {
    ManagmentGpio_outputEnable();
    ManagmentGpio_write(0);
    
    enableHkSpi(0);
    
    GPIOs_configure(5, GPIO_MODE_USER_STD_OUTPUT);
    GPIOs_configure(6, GPIO_MODE_USER_STD_OUTPUT);
    GPIOs_configure(7, GPIO_MODE_USER_STD_INPUT_PULLUP);
    GPIOs_configure(8, GPIO_MODE_USER_STD_OUTPUT);
    
    GPIOs_configure(12, GPIO_MODE_USER_STD_OUTPUT);
    GPIOs_configure(13, GPIO_MODE_USER_STD_OUTPUT);
    GPIOs_configure(14, GPIO_MODE_USER_STD_INPUT_PULLUP);
    GPIOs_configure(15, GPIO_MODE_USER_STD_OUTPUT);
    
    GPIOs_configure(19, GPIO_MODE_USER_STD_BIDIRECTIONAL);
    GPIOs_configure(20, GPIO_MODE_USER_STD_BIDIRECTIONAL);
    
    GPIOs_configure(23, GPIO_MODE_USER_STD_BIDIRECTIONAL);
    GPIOs_configure(24, GPIO_MODE_USER_STD_BIDIRECTIONAL);
    
    GPIOs_loadConfigs();
    
    User_enableIF();
    
    ManagmentGpio_write(1);
    
    return;
}
