/**
******************************************************************************
* @file         tcp_server.h                                 
* @version      V1.0										  		
* @date         2018-06-18								 
* @brief        tcp_server.c��ͷ�ļ�	
*
* @company      ��������Ƽ����޹�˾
* @information  WIZnet W5100S�ٷ������̣�ȫ�̼���֧�֣��۸�������ƣ�
* @website      www.wisioe.com																							 	 																												 		
* @qqGroup      383035001																										 
******************************************************************************
*/
#ifndef _TCP_ERVER_H_
#define _TCP_ERVER_H_

#include <stdint.h>

/* DATA_BUF_SIZE define for Loopback example */
#ifndef DATA_BUF_SIZE
#define DATA_BUF_SIZE			2048
#endif

/* TCP server Loopback test example */
void do_tcp_server(void);

#endif
