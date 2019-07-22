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


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define BUFFER_LEN 4


flash_config_t flashDriver;                                            /* Flash driver Structure */
flash_security_state_t securityStatus = kFLASH_SecurityStateNotSecure; /* Return protection status */

#if 0
uint32_t destAdrss; /* Address of the target location */
uint32_t i, failAddr, failDat;
uint32_t buffer[BUFFER_LEN]; /* Buffer for program */
#endif

uint32_t i, failAddr, failDat;

uint32_t pflashBlockBase = 0;
uint32_t pflashTotalSize = 0;
uint32_t pflashSectorSize = 0;
uint32_t dflashTotalSize = 0;
uint32_t dflashBlockBase = 0;
uint32_t eepromTotalSize = 0;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);
static void flash_erase_hal(uint32_t destAdrss);
static void flash_write_hal(uint32_t destAdrss, uint32_t *buffer, uint32_t len);


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Use Standard Software Drivers (SSD) to modify flash.
 *
 * @details This function uses SSD to demonstrate flash mode:
 *            + Check flash information.
 *            + Erase a sector and verify.
 *            + Program a sector and verify.
 */
status_t plc_flash_init(void)
{
	//uint32_t destAdrss = 0, i = 0;
	//uint32_t buffer[BUFFER_LEN] = {0};
    status_t result;    /* Return code from each flash driver function */
    /* Clean up Flash driver Structure*/
    memset(&flashDriver, 0, sizeof(flash_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&flashDriver);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
	    

    /* Get flash properties*/
    FLASH_GetProperty(&flashDriver, kFLASH_PropertyPflashBlockBaseAddr, &pflashBlockBase);
    FLASH_GetProperty(&flashDriver, kFLASH_PropertyPflashTotalSize, &pflashTotalSize);
    FLASH_GetProperty(&flashDriver, kFLASH_PropertyPflashSectorSize, &pflashSectorSize);
    //FLASH_GetProperty(&flashDriver, kFLASH_PropertyDflashTotalSize, &dflashTotalSize);
    //FLASH_GetProperty(&flashDriver, kFLASH_PropertyDflashBlockBaseAddr, &dflashBlockBase);
    //FLASH_GetProperty(&flashDriver, kFLASH_PropertyEepromTotalSize, &eepromTotalSize);
	
    return result;

}



/*
* @brief Gets called when an error occurs.
*
* @details Print error message and trap forever.
*/
void error_trap(void)
{
    //PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (1)
    {
    }
}

void PLC_ERASE_FLASH(uint32_t destAdrss, uint32_t lengthInByte)
{
	uint32_t i = 0, block = 0;
	//status_t result;    /* Return code from each flash driver function */

	if((lengthInByte%pflashSectorSize)!= 0)
	{
		block = ((lengthInByte/pflashSectorSize)+1);
	}
	else
	{
		block = lengthInByte/pflashSectorSize;
	}

	for(i = 0; i < block; i++)
	{
		flash_erase_hal((destAdrss+i*pflashSectorSize));
	}
}

void PLC_WRITE_FLASH(uint32_t destAdrss, uint32_t *buffer, uint32_t len)
{
	flash_write_hal(destAdrss, buffer, len);
}

void PLC_WRITE_ADDITION(uint32_t num, uint32_t *buffer)
{
	uint32_t destAdrss = 0;
	destAdrss = pflashBlockBase + (pflashTotalSize - num*pflashSectorSize);
	flash_erase_hal(destAdrss);
	flash_write_hal(destAdrss, buffer, sizeof(uint64_t));
}

void PLC_READ_ADDITION(uint32_t num, uint64_t *data)
{
	uint32_t destAdrss = 0;
	destAdrss = pflashBlockBase + (pflashTotalSize - num*pflashSectorSize);
	*data = *(uint64_t *)destAdrss;
}



static void flash_erase_hal(uint32_t destAdrss)
{
	status_t result;    /* Return code from each flash driver function */
	result = FLASH_Erase(&flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
	if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
}


static void flash_write_hal(uint32_t destAdrss, uint32_t *buffer, uint32_t len)
{
	status_t result;	/* Return code from each flash driver function */

	result = FLASH_Program(&flashDriver, destAdrss, (uint8_t *)buffer, len);
	if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
}
