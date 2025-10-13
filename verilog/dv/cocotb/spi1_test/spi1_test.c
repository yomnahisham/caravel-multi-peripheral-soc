#include <firmware_apis.h>
#include "CF_SPI.h"

#define SPI1_BASE 0x30010000

void main() {
    ManagmentGpio_outputEnable();
    ManagmentGpio_write(0);
    
    enableHkSpi(0);
    
    GPIOs_configure(12, GPIO_MODE_USER_STD_OUTPUT);
    GPIOs_configure(13, GPIO_MODE_USER_STD_OUTPUT);
    GPIOs_configure(14, GPIO_MODE_USER_STD_INPUT_PULLUP);
    GPIOs_configure(15, GPIO_MODE_USER_STD_OUTPUT);
    
    GPIOs_loadConfigs();
    
    User_enableIF();
    
    ManagmentGpio_write(1);
    
    CF_SPI_setGclkEnable(SPI1_BASE, 1);
    CF_SPI_enable(SPI1_BASE);
    CF_SPI_setPrescaler(SPI1_BASE, 8);
    CF_SPI_writepolarity(SPI1_BASE, 0);
    CF_SPI_writePhase(SPI1_BASE, 0);
    CF_SPI_enableRx(SPI1_BASE);
    
    CF_SPI_assertCs(SPI1_BASE);
    
    CF_SPI_writeData(SPI1_BASE, 0x5A);
    CF_SPI_waitTxFifoEmpty(SPI1_BASE);
    CF_SPI_waitNotBusy(SPI1_BASE);
    
    CF_SPI_deassertCs(SPI1_BASE);
    
    return;
}
