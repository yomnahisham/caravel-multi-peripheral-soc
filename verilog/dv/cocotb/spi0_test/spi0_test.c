#include <firmware_apis.h>
#include "CF_SPI.h"

#define SPI0_BASE 0x30000000

void main() {
    ManagmentGpio_outputEnable();
    ManagmentGpio_write(0);
    
    enableHkSpi(0);
    
    GPIOs_configure(5, GPIO_MODE_USER_STD_OUTPUT);
    GPIOs_configure(6, GPIO_MODE_USER_STD_OUTPUT);
    GPIOs_configure(7, GPIO_MODE_USER_STD_INPUT_PULLUP);
    GPIOs_configure(8, GPIO_MODE_USER_STD_OUTPUT);
    
    GPIOs_loadConfigs();
    
    User_enableIF();
    
    ManagmentGpio_write(1);
    
    CF_SPI_setGclkEnable(SPI0_BASE, 1);
    CF_SPI_enable(SPI0_BASE);
    CF_SPI_setPrescaler(SPI0_BASE, 8);
    CF_SPI_writepolarity(SPI0_BASE, 0);
    CF_SPI_writePhase(SPI0_BASE, 0);
    CF_SPI_enableRx(SPI0_BASE);
    
    CF_SPI_assertCs(SPI0_BASE);
    
    CF_SPI_writeData(SPI0_BASE, 0xA5);
    CF_SPI_waitTxFifoEmpty(SPI0_BASE);
    CF_SPI_waitNotBusy(SPI0_BASE);
    
    CF_SPI_deassertCs(SPI0_BASE);
    
    return;
}
