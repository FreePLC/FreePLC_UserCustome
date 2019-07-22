/**
******************************************************************************
* @file         tcp_client.h                                 
* @version      V1.0										  		
* @date         2018-06-18								 
* @brief        tcp_client.c��ͷ�ļ�	
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
* @website      www.wisioe.com																							 	 																												 		
* @qqGroup      383035001																										 
******************************************************************************
*/
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <stdint.h>

/* DATA_BUF_SIZE define for Loopback example */
#ifndef DATA_BUF_SIZE
#define DATA_BUF_SIZE			2048
#endif

/* TCP client Loopback test example */
void do_tcp_client(void);

#endif
