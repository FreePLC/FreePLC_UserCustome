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


#include "Plc_userCode.h"
#include "Plc_driver.h"
#include "Modbus.h"

#ifdef MODBUS_MASTER_USED
#include "ModbusMasterApp.h"
#endif



uint8_t *g_u8InputData; 
uint8_t *g_u8OutputData; 
uint8_t *g_u8MemoryData;



/*���´����û������޸�*/

/*************************************************************
������:void plc_get_input()
�������:��
����ֵ:��
����˵��:�ú���ÿ��IEC��������֮ǰ��
ִ��һ�Σ���ȡ���ݵ�I����������g_u8InputData
**************************************************************/


void plc_get_input()
{
  g_u8InputData[0] = DI1_IN|(DI2_IN<<1); 
}


/*************************************************************
������:void plc_set_output()
�������:��
����ֵ:��
����˵��:�ú���ÿ��IEC��������֮���
ִ��һ�Σ�����IEC����������������ܽ�
**************************************************************/

void plc_set_output()
{
  DO1_OUT = g_u8OutputData[0] & 0x01;
  DO2_OUT = (g_u8OutputData[0] >> 1) & 0x01;
  DO3_OUT = (g_u8OutputData[0] >> 2) & 0x01;
  DO4_OUT = (g_u8OutputData[0] >> 3) & 0x01;
  DO5_OUT = (g_u8OutputData[0] >> 4) & 0x01;
  DO6_OUT = (g_u8OutputData[0] >> 5) & 0x01;
  DO7_OUT = (g_u8OutputData[0] >> 6) & 0x01;
  DO8_OUT = (g_u8OutputData[0] >> 7) & 0x01;
  DO9_OUT = g_u8OutputData[1] & 0x01;
}

/*************************************************************
������:void plc_user_init()
�������:��
����ֵ:��
����˵��:�û���ʼ�����룬�ó���ÿ�θ�λ��
��ִ��һ�Σ�ͨ�����ڳ�ʼ������
**************************************************************/

void plc_user_init()
{
  GPIO_Configuration();
  ModbusSlaveInitPort(0);
  
#ifdef MODBUS_SLAVE_USED
  ModbusSlaveInitPort(0);
#endif
  
#ifdef MODBUS_MASTER_USED
  MB_Init();
  ModbusMasterInitPort(0);
#endif
  
}

/*************************************************************
������:void plc_user_main()
�������:��
����ֵ:��
����˵��:�û�����ѭ����
**************************************************************/

void plc_user_main()
{
#ifdef MODBUS_SLAVE_USED
  ModbusSlaveMainProcess(SLAVE_PORT0);
  ModbusSlavePollSend(SLAVE_PORT0);
#endif
  
#ifdef MODBUS_MASTER_USED
  ModbusNet1MasterAPP();
  ModbusMasterSendMessage(MASTER_PORT0);
  ModbusMasterMainReceive(MASTER_PORT0);
#endif
  
}

