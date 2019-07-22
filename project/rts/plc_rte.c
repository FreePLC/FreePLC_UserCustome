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

#include "Plc_abi.h"
#include "Plc_flash.h"
#include "Plc_driver.h"


#define PLC_RTE_MAGIC_BASE		0x29FFC
#define PLC_RTE_MAGIC_ID		0x123455AA



__root const uint32_t g_u32PLC_RTE_MAGIC@0x29FFC = PLC_RTE_MAGIC_ID;


extern uint8_t *g_u8InputData; 
extern uint8_t *g_u8OutputData; 
extern uint8_t *g_u8MemoryData;


extern void PLC_GetTime(IEC_TIME *CURRENT_TIME);
extern void PLC_SetTimer( uint64_t tick_next, uint64_t tick_period);
extern int plc_backup_check(void);
extern void plc_backup_invalidate(void);
extern void plc_backup_validate(void);
extern void plc_backup_retain(unsigned int offset, unsigned int count, void *p);
extern void plc_backup_remind(unsigned int offset, unsigned int count, void *p);
extern void plc_init_dio();
extern void plc_get_input();
extern void plc_set_output();
extern void plc_user_init();
extern void plc_user_main();
extern status_t plc_flash_init(void);


void plc_init_dio()
{
  g_u8InputData = (uint8_t *)PLC_INPUT_START_ADDR;
  g_u8OutputData = (uint8_t *)PLC_OUTPUT_START_ADDR;
  g_u8MemoryData = (uint8_t *)PLC_MEMORY_START_ADDR;
  plc_flash_init();

}


__root const plc_rte_abi_t plc_glue_rte @".plc_rte_sec" =
{
    .get_time  = PLC_GetTime,
    .set_timer = PLC_SetTimer,

    .check_retain_buf      = plc_backup_check,
    .invalidate_retain_buf = plc_backup_invalidate,
    .validate_retain_buf   = plc_backup_validate,

    .retain = plc_backup_retain,
    .remind = plc_backup_remind,

    .init_dio = plc_init_dio,
    .get_input = plc_get_input,
    .set_output = plc_set_output,

	/*************用户可以添加自己的RTE程序************************/

	.usr_init = plc_user_init,
	.usr_main = plc_user_main

};

