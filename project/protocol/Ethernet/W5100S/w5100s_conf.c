/**
******************************************************************************
* @file         w5100s_conf.c                                                                                              
* @version      V1.0                          
* @date         2018-06-18             
* @brief        ����MCU����ֲw5100s������Ҫ�޸ĵ��ļ�������w5100s��MAC��IP��ַ   
*        
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100s�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
* @website      www.wisioe.com                                                                                                               
* @qqGroup      383035001                                                     
******************************************************************************
*/

#include <stdio.h> 
#include <string.h>
#include "w5100s_conf.h"
#include "utility.h"
#include "w5100s.h"
#include "socket.h"
#include "dhcp.h"

CONFIG_MSG  ConfigMsg;                   // ���ýṹ��

uint16 rxlen=0;
uint8 rxbuf[1024];

uint8  txsize[MAX_SOCK_NUM] = {2,2,2,2}; // tx buffer set K bytes
uint8  rxsize[MAX_SOCK_NUM] = {2,2,2,2}; // rx buffet set K bytes
/*MAC��ַ���ֽڱ���Ϊż��
  ������W5100s�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ
*/
uint8 mac[6]={0x00,0x08,0xdc,0x22,0x33,0x33};

/*����Ĭ��IP��Ϣ*/
uint8 local_ip[4] = {192,168,1,233};                        // ����W5100sĬ��IP��ַ
uint8 subnet[4] = {255,255,255,0};                          // ����W5100sĬ����������
uint8 gateway[4] = {192,168,1,1};                           // ����W5100sĬ������
uint8 dns_server[4] = {114,114,114,114};                    // ����W5100sĬ��DNS

uint16 local_port = 8080;                                   // ���屾�ض˿�

/*����Զ��IP��Ϣ*/
uint8  remote_ip[4] = {192,168,1,100};                      // Զ��IP��ַ
uint16 remote_port = 8080;                                  // Զ�˶˿ں�

/*IP���÷���ѡ��������ѡ��*/
uint8  ip_from=IP_FROM_DEFINE;      

uint8  dhcp_ok=0;                                           // dhcp�ɹ���ȡIP
uint32 ms=0;                                                // �������
uint32 dhcp_time= 0;                                        // DHCP���м���
uint8    ntptimer = 0;                                        // NPT�����
/**
*@brief  ��ʼ��W5100s�ĸ�λ���жϹܽ�
*@param  ��
*@return ��
*/
void reset_break_gpio_init(void)
{
	;
}

/**
*@brief  ����W5100s��IP��ַ
*@param  ��
*@return ��
*/
void set_w5100s_netinfo(void)
{
  //uint8 temp;
  // ���ƶ����������Ϣ�����ýṹ��
  memcpy(ConfigMsg.mac, mac, 6);
  memcpy(ConfigMsg.lip,local_ip,4);
  memcpy(ConfigMsg.sub,subnet,4);
  memcpy(ConfigMsg.gw,gateway,4);
  memcpy(ConfigMsg.dns,dns_server,4);
  
  switch(ip_from)
  {
    case IP_FROM_DEFINE:
      printf(" ʹ�ö����IP��Ϣ����W5100S��\r\n");
      break;
      
    case IP_FROM_DHCP:
      // ����DHCP��ȡ��������Ϣ�����ýṹ��
      if(dhcp_ok==1)
      {
        printf(" IP from DHCP\r\n");     
        memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
        memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
        memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
        memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
      }
      else
      {
        printf(" DHCP�ӳ���δ����,���߲��ɹ�\r\n");
        printf(" ʹ�ö����IP��Ϣ����W5100S\r\n");
      }
      break;
      
    case IP_FROM_FLASH:
      ;
      break;
    }
  setSUBR(ConfigMsg.sub);
  setGAR(ConfigMsg.gw);
  setSIPR(ConfigMsg.lip);

  getSIPR (local_ip);      
  printf(" W5100S IP��ַ   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
  getSUBR(subnet);
  printf(" W5100S �������� : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
  getGAR(gateway);
  printf(" W5100S ����     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
*@brief  ����W5100S��MAC��ַ
*@param  ��
*@return ��
*/
void set_w5100s_mac(void)
{
  // ����������Ϣ��������Ҫѡ��  
  ConfigMsg.sw_ver[0]=FW_VER_HIGH;
  ConfigMsg.sw_ver[1]=FW_VER_LOW;  
  
  memcpy(ConfigMsg.mac, mac, 6);
  setSHAR(ConfigMsg.mac);
  memcpy(DHCP_GET.mac, mac, 6);
  getSHAR(mac);
  printf(" W5100S MAC��ַ  : %02x.%02x.%02x.%02x.%02x.%02x\r\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
}
  


/**
*@brief  dhcp�õ��Ķ�ʱ����ʼ��
*@param  ��
*@return ��
*/
void dhcp_timer_init(void)
{
  //timer2_init();                                  
}

/**
*@brief  ntp�õ��Ķ�ʱ����ʼ��
*@param  ��
*@return ��
*/
void ntp_timer_init(void)
{
  //timer2_init();                                  
}

/**
*@brief  ��ʱ��2��ʼ��
*@param  ��
*@return ��
*/
void timer2_init(void)
{
	;
}

/**
*@brief  ��ʱ��2�жϺ���
*@param  ��
*@return ��
*/
void timer2_isr(void)
{
  ms++;  
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;             // DHCP��ʱ��1S
#ifndef __NTP_H__
    ntptimer++;              // NTP����ʱ���1S
#endif
  }
}

/**
*@brief  ������������
*@param  ��
*@return ��
*/
void PHY_check(void)
{
  uint8 PHY_connect=0;
  PHY_connect=0x01&getPHYStatus();
  if(PHY_connect==0)
  {
    printf(" \r\n ���������Ƿ�����?\r\n");
    PHY_connect = 0;
    do
    {
      close(PHY_connect++);
    }while(PHY_connect < 4);
    PHY_connect = 0;
    while(PHY_connect == 0) { 
      PHY_connect=0x01&getPHYStatus();  
      printf(" .");
      delay_ms(500);
    }
    printf(" \r\n");  
  }
}

/**
*@brief  ��ȡPHY�Ĵ�������
*@param  ��
*@return ��
*/
uint8 getPHYStatus( void )
{
  return IINCHIP_READ(PHYSR);
}
