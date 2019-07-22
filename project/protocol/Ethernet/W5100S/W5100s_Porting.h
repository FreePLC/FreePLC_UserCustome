#ifndef _W5100S_PORTING_H_
#define _W5100S_PORTING_H_

#include "fsl_common.h"
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"



void SPI_PinConfiguration();
void SPI_Configuration();
uint8_t spi_read_byte(void);
void spi_send_byte(uint8_t data);
void cs_high(void);
void cs_low(void);
void reset_w5100s(void);
void W5100s_Init();


#endif

