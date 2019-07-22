/**
******************************************************************************
* @file         tcp_server.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        tcp���������ݻػ�����        
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
* @website      www.wisioe.com                                                                                                               
* @qqGroup      383035001                                                     
******************************************************************************
*/
#include <stdio.h>
#include "tcp_server.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5100s_conf.h"

uint8_t buff[DATA_BUF_SIZE];  

void do_tcp_server(void)
{  
  uint16 len=0;  
  switch(getSn_SR(SOCK_TCPS))                             // ��ȡsocket��״̬
  {
    case SOCK_CLOSED:                                     // socket���ڹر�״̬
      socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);   // ��socket
      break;     
    
    case SOCK_INIT:                                       // socket�ѳ�ʼ��״̬
      listen(SOCK_TCPS);                                  // socket��������
      break;
    
    case SOCK_ESTABLISHED:                                // socket�������ӽ���״̬
    
      if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
      {
        setSn_IR(SOCK_TCPS, Sn_IR_CON);                   // ��������жϱ�־λ
      }
      len=getSn_RX_RSR(SOCK_TCPS);                        // ����lenΪ�ѽ������ݵĳ���
      if(len>0)
      {
        recv(SOCK_TCPS,buff,len);                         // ��������Client������
        buff[len]=0x00;                                   // ����ַ���������
        printf("%s\r\n",buff);
        send(SOCK_TCPS,buff,len);                         // ��Client��������
      }
      break;
    
    case SOCK_CLOSE_WAIT:                                 // socket���ڵȴ��ر�״̬
      close(SOCK_TCPS);
      break;
  }
}
