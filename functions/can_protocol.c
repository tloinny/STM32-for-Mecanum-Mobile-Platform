/**
 *@title Abot Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Abot]
 *
 *@created on 2019-1-08  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: Abot�ؽڿ��ƽڵ�CAN����ͨѶЭ������
 */

#include "sys_conf.h"

u8 can_send_buf[can_buf_size] = {0};	/* CAN���ͻ����� */
u8 can_rec_buf[can_buf_size] = {0};	/* CAN���ջ����� */

/**
 *@function CAN���������ͷ�����Ϣ
 *@param 
 *				feedback:������Ϣ
 *@return 
 *				0----�ɹ�
 *				����----ʧ��
 */
u8 CAN_send_feedback(u8 *feedback)
{
	u8 result;
	result = Can_Send_Msg(feedback, 3, master);
	return result;
}

/**
*@function ���CAN���ͻ���������
 *@param void
 *@return void
 */
void clean_can_send_buf()
{
	int i = 0;
	for(;i<can_buf_size;++i)
	{
		can_send_buf[i] = 0;
	}
}
