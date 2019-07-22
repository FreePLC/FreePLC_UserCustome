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


#include "plc_driver.h"

/*******************************************************************************
* Definitions
******************************************************************************/


/*******************************************************************************
* Prototypes
******************************************************************************/


/*******************************************************************************
* Variables
******************************************************************************/
//APP

static void WaitWctClose(WDOG_Type *base);


/*******************************************************************************
* Code
******************************************************************************/

void GPIO_Configuration()
{
  
  gpio_pin_config_t input_config = {
	kGPIO_DigitalInput, 0,
  };
  
  gpio_pin_config_t output_config = {
	kGPIO_DigitalOutput, 0,
  };
  
  port_pin_config_t pin_config;
  
  port_digital_filter_config_t filter_config;
  
  pin_config.pullSelect = kPORT_PullUp;
  pin_config.slewRate = kPORT_SlowSlewRate;
  pin_config.driveStrength = kPORT_LowDriveStrength;
  pin_config.lockRegister = kPORT_UnlockRegister;
  pin_config.openDrainEnable = kPORT_OpenDrainDisable;
  pin_config.mux = kPORT_MuxAsGpio;
  pin_config.passiveFilterEnable = kPORT_PassiveFilterDisable;
  
  filter_config.clockSource = kPORT_LpoClock;
  filter_config.digitalFilterWidth = 0x1F;	  
  
  
  PORT_SetPinConfig(DI1_PORT, DI1_PIN, &pin_config);
  GPIO_PinInit(DI1_GPIO, DI1_PIN, &input_config);
  
  
  PORT_SetPinConfig(DI2_PORT, DI2_PIN, &pin_config);
  GPIO_PinInit(DI2_GPIO, DI2_PIN, &input_config);
    
  
  //Output
  
  PORT_SetPinConfig(DO1_PORT, DO1_PIN, &pin_config);  
  GPIO_PinInit(DO1_GPIO, DO1_PIN, &output_config);
  
  PORT_SetPinConfig(DO2_PORT, DO2_PIN, &pin_config);  
  GPIO_PinInit(DO2_GPIO, DO2_PIN, &output_config);
  
  PORT_SetPinConfig(DO3_PORT, DO3_PIN, &pin_config);  
  GPIO_PinInit(DO3_GPIO, DO3_PIN, &output_config);
  
  PORT_SetPinConfig(DO4_PORT, DO4_PIN, &pin_config);  
  GPIO_PinInit(DO4_GPIO, DO4_PIN, &output_config);
  
  PORT_SetPinConfig(DO5_PORT, DO5_PIN, &pin_config);  
  GPIO_PinInit(DO5_GPIO, DO5_PIN, &output_config);
  
  PORT_SetPinConfig(DO6_PORT, DO6_PIN, &pin_config);  
  GPIO_PinInit(DO6_GPIO, DO6_PIN, &output_config);
  
  PORT_SetPinConfig(DO7_PORT, DO7_PIN, &pin_config);  
  GPIO_PinInit(DO7_GPIO, DO7_PIN, &output_config);
  
  PORT_SetPinConfig(DO8_PORT, DO8_PIN, &pin_config);  
  GPIO_PinInit(DO8_GPIO, DO8_PIN, &output_config);
  
  PORT_SetPinConfig(DO9_PORT, DO9_PIN, &pin_config);  
  GPIO_PinInit(DO9_GPIO, DO9_PIN, &output_config);
  
  
  //Input Filter
  PORT_SetDigitalFilterConfig(DI1_PORT, &filter_config);
  PORT_EnablePinsDigitalFilter(DI1_PORT, (1<<DI1_PIN), true);
  
  PORT_SetDigitalFilterConfig(DI2_PORT, &filter_config);
  PORT_EnablePinsDigitalFilter(DI2_PORT, (1<<DI2_PIN), true);

}



/*******************************************************************************
* WDT
******************************************************************************/

static void WaitWctClose(WDOG_Type *base)
{
  /* Accessing register by bus clock */
  for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
  {
    (void)base->RSTCNT;
  }
}


void WDOG_Configuration()
{
  wdog_config_t config;
  
  WDOG_GetDefaultConfig(&config);
  config.timeoutValue = 0x7ffU;
  
  WDOG_Init(WDOG, &config);
  WaitWctClose(WDOG);
  
}



