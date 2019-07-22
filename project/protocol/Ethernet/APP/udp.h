/**
******************************************************************************
* @file         udp.h                                 
* @version      V1.0                          
* @date         2018-06-18                 
* @brief        udp.c��ͷ�ļ�  
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
* @website      www.wisioe.com                                                                                                               
* @qqGroup      383035001                                                     
******************************************************************************
*/
#ifndef _UDP_H_
#define _UDP_H_

#include <stdint.h>

/* DATA_BUF_SIZE define for Loopback example */
#ifndef DATA_BUF_SIZE
#define DATA_BUF_SIZE  2048
#endif

/* UDP Loopback test example */
void do_udp(void);

#endif
