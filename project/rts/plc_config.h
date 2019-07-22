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

#ifndef _PLC_PARAMETER_H_
#define _PLC_PARAMETER_H_


#include "fsl_common.h"

/*************NOTE***********************************************
In the link file, we used plc_rte_sec to save the union function 
place at address mem: 0x00001000            { readonly section .plc_rte_sec };
*****************************************************************/
#define PLC_APP_BASE            0x20000
#define PLC_APP_BYTESIZE        0x20000
#define PLC_BLOCKNUM_BASE	1		//it mean the last sector
#define PLC_APP_CHECKSUM_BASE	2	//it mean the penult sector
#define PLC_RETAIN_CHECKSUM_B   4
#define PLC_RETAIN_CHECKSUM_A	3	//it mean the last 3,4 sector used to save the retain
#define PLC_RETAIN_FLAG_BASE    5

#define PLC_FLASH_WRITE_UNIT	8
//#define PLC_APP ((plc_app_abi_t *)PLC_APP_BASE)

#define disable_interrupt()     __set_PRIMASK(1)
#define enable_interrupt()      __set_PRIMASK(0)

#define MEMORY_M_SIZE           2000
#define MEMORY_I_SIZE
#define MEMORY_Q_SIZE
#define MEMORY_R_SIZE		2048



#define RETAIN_VALID_A          0x55
#define RETAIN_VALID_B          0xAA
#define RETAIN_VALID_NULL       0x00
#define RETAIN_WRITE_SIZE	16		//Byte
#define RETAIN_RETRY_TIME       3

#endif