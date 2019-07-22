/*
* The Clear BSD License
* Copyright (c) 2019 Liang.Yang
* Copyright 2019-2019 Liang.Yang <WeChat:kala4tgo><Email:17389711@qq.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted (subject to the limitations in the disclaimer below) provided
* that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _FSL_IH_DRIVER_H_
#define _FSL_IH_DRIVER_H_



#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <stddef.h>

#include "fsl_device_registers.h"
#include "fsl_pit.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_dspi.h"
#include "fsl_wdog.h"

  
  
  /*******************************************************************************
  * Definitions
  ******************************************************************************/
  
  
#define WDOG_WCT_INSTRUCITON_COUNT (256U)



//Clock

#define BOOT_BUS_CLOCK        (32768*640)         

//PIT
#define PIT_SOURCE_CLOCK  CLOCK_GetFreq(kCLOCK_BusClk)

//GPIO

#define DO1_GPIO              	GPIOD
#define DO1_PORT              	PORTD
#define DO1_PIN               	0

#define DO2_GPIO              	GPIOD
#define DO2_PORT              	PORTD
#define DO2_PIN               	1

#define DO3_GPIO              	GPIOD
#define DO3_PORT              	PORTD
#define DO3_PIN               	2

#define DO4_GPIO              	GPIOD
#define DO4_PORT              	PORTD
#define DO4_PIN               	3

#define DO5_GPIO              	GPIOD
#define DO5_PORT              	PORTD
#define DO5_PIN               	4

#define DO6_GPIO              	GPIOD
#define DO6_PORT              	PORTD
#define DO6_PIN               	5

#define DO7_GPIO              	GPIOC
#define DO7_PORT              	PORTC
#define DO7_PIN               	1

#define DO8_GPIO              	GPIOC
#define DO8_PORT              	PORTC
#define DO8_PIN               	2

#define DO9_GPIO              	GPIOB
#define DO9_PORT              	PORTB
#define DO9_PIN               	23


#define DI1_GPIO              	GPIOC
#define DI1_PORT              	PORTC
#define DI1_PIN               	7

#define DI2_GPIO              	GPIOE
#define DI2_PORT              	PORTE
#define DI2_PIN               	6

#define W5100_RST_GPIO			GPIOC
#define W5100_RST_PORT			PORTC
#define W5100_RST_PIN			3

#define W5100_IRQ_GPIO			GPIOA
#define W5100_IRQ_PORT			PORTA
#define W5100_IRQ_PIN			13

#define W5100_CS_GPIO			GPIOA
#define W5100_CS_PORT			PORTA
#define W5100_CS_PIN			14

#define W5100_SCK_GPIO			GPIOA
#define W5100_SCK_PORT			PORTA
#define W5100_SCK_PIN			15
	
#define W5100_MISO_GPIO			GPIOA
#define W5100_MISO_PORT			PORTA
#define W5100_MISO_PIN			17

#define W5100_MOSI_GPIO			GPIOA
#define W5100_MOSI_PORT			PORTA
#define W5100_MOSI_PIN			16


#define W5100_RST_OUT			BITBAND_REG(W5100_RST_GPIO->PDOR, W5100_RST_PIN)
#define W5100_CS_OUT			BITBAND_REG(W5100_CS_GPIO->PDOR, W5100_CS_PIN)	
#define W5100_IRQ_IN			BITBAND_REG(W5100_IRQ_GPIO->PDIR, W5100_IRQ_PIN)



#define DI1_IN 					BITBAND_REG(DI1_GPIO->PDIR, DI1_PIN)

#define DI2_IN 					BITBAND_REG(DI2_GPIO->PDIR, DI2_PIN)

#define DO1_OUT 				BITBAND_REG(DO1_GPIO->PDOR, DO1_PIN)	
#define DO1_NOT 				BITBAND_REG(DO1_GPIO->PTOR, DO1_PIN)

	
#define DO2_OUT 				BITBAND_REG(DO2_GPIO->PDOR, DO2_PIN)
#define DO2_NOT 				BITBAND_REG(DO2_GPIO->PTOR, DO2_PIN)

#define DO3_IN 					BITBAND_REG(DO3_GPIO->PDIR, DO3_PIN)
#define DO3_OUT 				BITBAND_REG(DO3_GPIO->PDOR, DO3_PIN)
#define DO3_NOT 				BITBAND_REG(DO3_GPIO->PTOR, DO3_PIN)


#define DO4_OUT 				BITBAND_REG(DO4_GPIO->PDOR, DO4_PIN)
#define DO4_NOT 				BITBAND_REG(DO4_GPIO->PTOR, DO4_PIN)

#define DO5_OUT 				BITBAND_REG(DO5_GPIO->PDOR, DO5_PIN)
#define DO5_NOT 				BITBAND_REG(DO5_GPIO->PTOR, DO5_PIN)

#define DO6_OUT 				BITBAND_REG(DO6_GPIO->PDOR, DO6_PIN)
#define DO6_NOT 				BITBAND_REG(DO6_GPIO->PTOR, DO6_PIN)

#define DO7_OUT 				BITBAND_REG(DO7_GPIO->PDOR, DO7_PIN)
#define DO7_NOT 				BITBAND_REG(DO7_GPIO->PTOR, DO7_PIN)

#define DO8_OUT 				BITBAND_REG(DO8_GPIO->PDOR, DO8_PIN)
#define DO8_NOT 				BITBAND_REG(DO8_GPIO->PTOR, DO8_PIN)

#define DO9_OUT 				BITBAND_REG(DO9_GPIO->PDOR, DO9_PIN)
#define DO9_NOT 				BITBAND_REG(DO9_GPIO->PTOR, DO9_PIN)

  


/*******************************************************************************
* API
******************************************************************************/


void WDOG_Configuration();
uint64_t FLASH_ExESector(uint64_t destination);
uint64_t FLASH_ProgramSectionByLongs(uint64_t destination, uint64_t* pSource, uint64_t size);
uint32_t FlashSignoff(void);
__ramfunc uint64_t FLASH_FlashCommandSequenceStart(uint8_t index);
uint64_t FLASH_EraseEASector(uint64_t destination);
uint64_t FLASH_EraseSector(uint64_t destination);

void GPIO_Configuration();


/*! @}*/

#endif /* _FSL_PIT_H_ */
