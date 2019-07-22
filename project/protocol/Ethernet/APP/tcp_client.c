/**
******************************************************************************
* @file         tcp_client.c                                         
* @version      V1.0										  		
* @date         2018-06-18								 
* @brief        tcp�ͻ������ݻػ�����		  	
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
* @website      www.wisioe.com																							 	 																												 		
* @qqGroup      383035001																										 
******************************************************************************
*/
#include <stdio.h>
#include "tcp_client.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5100s_conf.h"

uint8_t buff[DATA_BUF_SIZE];	

void do_tcp_client(void)
{
  uint16 len=0;	

  switch(getSn_SR(SOCK_TCPC))                           // ��ȡsocket��״̬
  {
    case SOCK_CLOSED:                                   // socket���ڹر�״̬
      socket(SOCK_TCPC,Sn_MR_TCP,local_port++,Sn_MR_ND);
      break;

    case SOCK_INIT:                                     // socket���ڳ�ʼ��״̬
      connect(SOCK_TCPC,remote_ip,remote_port);         // socket���ӷ����� 
      break;

    case SOCK_ESTABLISHED:                              // socket�������ӽ���״̬
      if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
      {
        setSn_IR(SOCK_TCPC, Sn_IR_CON);                 // ��������жϱ�־λ
      }

      len=getSn_RX_RSR(SOCK_TCPC);                      // ����lenΪ�ѽ������ݵĳ���
      if(len>0)
      {
        recv(SOCK_TCPC,buff,len);                       // ��������Server������
        buff[len]=0x00;                                 // ����ַ���������
        printf("%s\r\n",buff);
        send(SOCK_TCPC,buff,len);                       // ��Server��������
      }		  
      break;

    case SOCK_CLOSE_WAIT:                               // socket���ڵȴ��ر�״̬
      close(SOCK_TCPC);
      break;
  }
}
