#include <firmware_apis.h>
#include "CF_SPI.h"

#define SPI0_BASE 0x30000000
#define SPI1_BASE 0x30010000
#define I2C_BASE 0x30020000
#define GPIO_BASE 0x30030000

#define GPIO_DATAO_OFFSET 1
#define GPIO_DIR_OFFSET 2
#define GPIO_GCLK_OFFSET 16324

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
    
    CF_SPI_setGclkEnable(SPI0_BASE, 1);
    CF_SPI_enable(SPI0_BASE);
    CF_SPI_setPrescaler(SPI0_BASE, 8);
    
    CF_SPI_setGclkEnable(SPI1_BASE, 1);
    CF_SPI_enable(SPI1_BASE);
    CF_SPI_setPrescaler(SPI1_BASE, 8);
    
    USER_writeWord(0x00000001, GPIO_BASE + GPIO_GCLK_OFFSET);
    USER_writeWord(0x00000003, GPIO_BASE + GPIO_DIR_OFFSET);
    USER_writeWord(0x00000001, GPIO_BASE + GPIO_DATAO_OFFSET);
    
    CF_SPI_assertCs(SPI0_BASE);
    CF_SPI_writeData(SPI0_BASE, 0xAA);
    CF_SPI_waitTxFifoEmpty(SPI0_BASE);
    CF_SPI_deassertCs(SPI0_BASE);
    
    CF_SPI_assertCs(SPI1_BASE);
    CF_SPI_writeData(SPI1_BASE, 0x55);
    CF_SPI_waitTxFifoEmpty(SPI1_BASE);
    CF_SPI_deassertCs(SPI1_BASE);
    
    USER_writeWord(0x00000002, GPIO_BASE + GPIO_DATAO_OFFSET);
    
    return;
}
