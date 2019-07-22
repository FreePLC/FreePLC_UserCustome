/**
******************************************************************************
* @file         udp.c                                         
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        udpЭ�����ݻػ�����        
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
* @website      www.wisioe.com                                                                                                               
* @qqGroup      383035001                                                     
******************************************************************************
*/
#include <stdio.h>
#include "udp.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "w5100s_conf.h"

uint8_t buff[DATA_BUF_SIZE];  
void do_udp(void)
{
  uint8 len;
  switch(getSn_SR(SOCK_UDPS))                                     // ��ȡsocket��״̬
  {
    case SOCK_CLOSED:                                             // socket���ڹر�״̬
      socket(SOCK_UDPS,Sn_MR_UDP,local_port,0);                   // ��ʼ��socket
      break;    
    case SOCK_UDP:                                                // socket��ʼ�����
      if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
      {
        setSn_IR(SOCK_UDPS, Sn_IR_RECV);                          // ������ж�
      }
      if((len=getSn_RX_RSR(SOCK_UDPS))>0)                         // ���յ�����
      {
        buff[len-8]=0x00;                                         // ����ַ���������
        recvfrom(SOCK_UDPS,buff, len, remote_ip,&remote_port);    // W5500���ռ����������������                                                   
        printf("%s\r\n",buff);                                    // ��ӡ���ջ��� 
        sendto(SOCK_UDPS,buff,len-8, remote_ip, remote_port);     // W5500�ѽ��յ������ݷ��͸�Remote
      }
      break;  
  }
}
