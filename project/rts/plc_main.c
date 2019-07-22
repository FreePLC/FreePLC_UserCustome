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
#include "plc_driver.h"
#include "plc_config.h"
#include "plc_flash.h"



extern uint32_t pflashBlockBase;
extern uint32_t pflashTotalSize;
extern uint32_t pflashSectorSize;


uint64_t *g_u64timer = (uint64_t *)0x1FFFC000;

uint8_t g_u8RetainBuffer[MEMORY_R_SIZE];
uint32_t g_AoData = 0, g_AoNum = 0;
uint32_t g_RetainAB = 0;					//Retain 区有AB两个flash pingpang操作

static void plc_checkum_retain(uint32_t *addr);
static uint32_t plc_checksum_verify(uint32_t *addr);
static uint32_t GenCrcCode(uint8_t *datapt,int bytecount);


//MODBUS通讯协议中CRC校验码算法
static uint32_t GenCrcCode(uint8_t *datapt,int bytecount)
{
	unsigned int  Reg16=0xFFFF,mval=0xA001;  //0xA001=1010000000000001
	int   i;
//	WORD  chkcode;
	char  j,flg; 
	for(i=0; i<bytecount; i++)
	{
		Reg16^=*(datapt+i);
		for(j=0;j<8;j++)
		{
			flg = 0;
			flg = (char)(Reg16&0x0001);  //get b0 value
			Reg16>>=1;         //right shift 1 bit ,set b15=0
			if(flg==1)
            {
				Reg16=Reg16^mval;
            }
		}
	}
//	printf("crccode=%x",Reg16);
	return(Reg16);
}
//目前运行
void PLC_GetTime(IEC_TIME *CURRENT_TIME)
{
  CURRENT_TIME->tv_sec = *(uint32_t *)0x1FFFC008;//*g_u64timer;
  CURRENT_TIME->tv_nsec = (*(uint64_t *)0x1FFFC000)*1000000;
}

void PLC_SetTimer( uint64_t tick_next, uint64_t tick_period )
{
  (void)tick_next;

  *(uint32_t *)0x1FFFC010 = (tick_period/1000000);
  
}

//判断retain区是否正常，如果正常则IEC会调用plc_backup_remind恢复retain
int plc_backup_check(void)
{

  uint32_t retain_flag = 0, ret = 0, flash_addr_a = 0, flash_addr_b = 0, valid_a = 0, valid_b = 0;
  
  retain_flag = *(uint32_t *)(pflashBlockBase + (pflashTotalSize - PLC_RETAIN_FLAG_BASE*pflashSectorSize));
  
  flash_addr_a = pflashBlockBase + (pflashTotalSize - PLC_RETAIN_CHECKSUM_A*pflashSectorSize);
  if(plc_checksum_verify((uint32_t *)flash_addr_a))
  {
    valid_a = 1;
    
  }
  else
  {
    ;
  }
  
  flash_addr_b = pflashBlockBase + (pflashTotalSize - PLC_RETAIN_CHECKSUM_B*pflashSectorSize);
  if(plc_checksum_verify((uint32_t *)flash_addr_b))
  {
    valid_b = 1; 
  }
  else
  {
    ;
  } 
 
  if((retain_flag == RETAIN_VALID_A)&&(valid_a == 1))
  {
    g_RetainAB = RETAIN_VALID_A;
    ret = 1;
  }
  else if((retain_flag == RETAIN_VALID_B)&&(valid_b == 1))
  {
    
    g_RetainAB = RETAIN_VALID_B; 
    ret = 1;		
  }
  else
  {
    if(valid_a == 1)
    {
      g_RetainAB = RETAIN_VALID_A;
      ret = 1;
      
    }
    else if(valid_b == 1)
    {
      g_RetainAB = RETAIN_VALID_B;
      ret = 1;
      
    }
    else
    {
      ret = 0;
    }
  }

  if(g_RetainAB == RETAIN_VALID_A)
  {
  		memcpy((void *)g_u8RetainBuffer, (void *)flash_addr_a, MEMORY_R_SIZE);
  }
  else if(g_RetainAB == RETAIN_VALID_B)
  {
		memcpy((void *)g_u8RetainBuffer, (void *)flash_addr_b, MEMORY_R_SIZE);
  }
  
  return ret;
  

  
}

//每次IEC会调用一次，暂时没用
void plc_backup_invalidate(void)
{

}

static void plc_checkum_retain(uint32_t *addr)
{
	//uint32_t i = 0;
	uint32_t *pflash, checksum = 0;
	
	pflash = addr;
	
	checksum = GenCrcCode((uint8_t *)pflash, (MEMORY_R_SIZE - 4));
	
	pflash = pflash + ((MEMORY_R_SIZE/4) - 1);
	
	if(pflash > addr)
	{
	  *pflash = checksum;
	}
	else
	{
	  ;
	}

}

static uint32_t plc_checksum_verify(uint32_t *addr)
{
	uint32_t ret = 0;
	uint32_t *pflash, checksum = 0;
	
	pflash = addr;
	
	checksum = GenCrcCode((uint8_t *)pflash, (MEMORY_R_SIZE - 4));
	
	pflash = pflash + ((MEMORY_R_SIZE/4) - 1);
	
	if(pflash > addr)
	{
		  if(checksum == *pflash)
		  {
			  ret = 1;
		  }
		  else
		  {
			  ret = 0;
		  }
	}
	else
	{
		  ret = 0;
	}
	
	return ret;
	

  
}

//实际写入非易失性存储器
void plc_backup_validate(void)
{
	static uint32_t write_count = 0, retain_chg = 0;
	static uint32_t flash_addr = 0, rery = 0;
	
	uint32_t ucCount = 0;//, checksum = 0;
	uint64_t *pflash,*pram;
	
	pram = (uint64_t *)g_u8RetainBuffer;
	
	if(retain_chg != 1)
	{
	  if(g_RetainAB != RETAIN_VALID_B)
	  {
		pflash = (uint64_t *) (pflashBlockBase + (pflashTotalSize - PLC_RETAIN_CHECKSUM_A*pflashSectorSize));
	  }
	  else 
	  {
		pflash = (uint64_t *) (pflashBlockBase + (pflashTotalSize - PLC_RETAIN_CHECKSUM_B*pflashSectorSize));
	  }
	  //判断Retain是否变化
	  for(ucCount=0; ucCount < (MEMORY_R_SIZE/8); ucCount++)
	  {
		if(*(pram) != *(pflash))								  //数据不一致则置更新标志
		{
		  retain_chg = 1;										  //记录发生变化标志
		  if(g_RetainAB != RETAIN_VALID_A)
		  {
			flash_addr = pflashBlockBase + (pflashTotalSize - PLC_RETAIN_CHECKSUM_A*pflashSectorSize);
			plc_checkum_retain((uint32_t *)&g_u8RetainBuffer);
		  }
		  else
		  {
			flash_addr = pflashBlockBase + (pflashTotalSize - PLC_RETAIN_CHECKSUM_B*pflashSectorSize);
			plc_checkum_retain((uint32_t *)&g_u8RetainBuffer);
		  }
		  write_count = 0;
		  break;
		}
		ucCount++;
		pram++;
		pflash++;
		
	  }
	
	  if(retain_chg == 1)
	  {
		  PLC_ERASE_FLASH(flash_addr, MEMORY_R_SIZE);
	  }
	  
	}
	else
	{
	  PLC_WRITE_FLASH(flash_addr+write_count*RETAIN_WRITE_SIZE, (uint32_t *)&g_u8RetainBuffer[write_count*RETAIN_WRITE_SIZE], RETAIN_WRITE_SIZE);
	  write_count++;
	  if(write_count > (MEMORY_R_SIZE/RETAIN_WRITE_SIZE))
	  {
		if(plc_checksum_verify((uint32_t *)flash_addr))
		{
		  flash_addr = 0;
		  
		  if(g_RetainAB != RETAIN_VALID_A)
		  {
			g_RetainAB = RETAIN_VALID_A;
		  }
		  else
		  {
			g_RetainAB = RETAIN_VALID_B;
		  }
		  
		  PLC_WRITE_ADDITION(PLC_RETAIN_FLAG_BASE, &g_RetainAB);
		  
		  retain_chg = 0;
		}
		else
		{
		  rery++;
		  write_count = 0;
		  
		  if(rery >= RETAIN_RETRY_TIME)
		  {
			flash_addr = 0;
			retain_chg = 0;
		  }
		}
	  }
	}

}

void plc_backup_retain(unsigned int offset, unsigned int count, void *p)
{
	if (offset + count < MEMORY_R_SIZE)
	{
	  memcpy((void *)(g_u8RetainBuffer + offset), p, count);
	}

  
}

void plc_backup_remind(unsigned int offset, unsigned int count, void *p)
{
	if (offset + count < MEMORY_R_SIZE)
	{
	  memcpy( p, (void *)(g_u8RetainBuffer + offset), count);
	}

}






