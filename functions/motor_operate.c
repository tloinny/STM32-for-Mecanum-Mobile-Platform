/**
 *@title Abot Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Abot]
 *
 *@created on 2019-1-08  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description:	�������
 */

#include "sys_conf.h"

const float step_angle = 2 * pi/(motor_type*Micro_Step);	/* ����� */
const float timer_frep = 72000000/(psc_init+1);	/* ��ʱ��Ƶ�� */
u8 Motor_status = 0;	/* ���״̬��־ */
u8 zeroed = 0;	/* ��������־ */
int current_position = -1;	/* ���������̼� */
int motion_dir = 1;	/* ����˶������־ */

u32 pre_cndtr = 0;	/* ��һ�ε�CNDTRֵ */
u32 this_cndtr = 0;	/* ��ǰ��CNDTRֵ */

u16 send_buf[send_buf_size];	/* ARR���û����� */

/**
 *@function �����ʼ��
 *���Ӳ����ʼ��,��Ϊʵ�ֵ�����Ƽ���ʹ�õ���Դ���г�ʼ��
 *@param	
 *				DMA_CHx:�������ռ�õ�DMAͨ��
 *				cpar:�������ʹ�õĶ�ʱ��ARR�����ַ
 *				cmar:�洢����ַ
 *				cndtr:���ݴ�����
 *				arr:��ʱ���ĳ�ʼ�����װ��ֵ
 *				psc:��ʱ���ķ�Ƶ��
 *@return
 */
void motor_init(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr, u16 arr, u16 psc)
{
	motor_io_init();	/* ��ʼ��IO */
	DMA_Config(DMA_CHx, cpar, cmar, cndtr);	/* ��ʼ��DMA */
	TIM3_PWM_Init(arr, psc);	/* ��ʼ����ʱ�� */
	motor_enable();					/* ʹ�ܵ�� */
	Motor_status = m_waiting;	/* ��ʼ�����״̬Ϊ�ȴ�״̬ */
}

/**
 *@function ���ʹ��
 *@param void
 *@return void
 */
void motor_enable()
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);	
	Motor_status = m_waiting;
}

/**
 *@function ���ʧ��
 *@param void
 *@return void
 */
void motor_disable()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	Motor_status = m_waiting;
}

/**
 *@function ���ת��
 *@param 
*				dir: 0��1,ӳ��Ϊ����˶�����
 *@return void
 */
void motor_dir(u8 dir)
{
	dir ? (GPIO_SetBits(GPIOB, GPIO_Pin_3),motion_dir = 1) : (GPIO_ResetBits(GPIOB, GPIO_Pin_3),motion_dir = -1);
}

/**
 *@function ����˶����ƣ����ģ�AVR446��������μӼ����㷨
 *@param 
 *				steps:��������˶��Ĳ���
 *				dir:��������˶��ķ���
 *				speed_max:Ŀ���ٶ�
 *				speed_init:��ʼ�ٶ�
 *				acc_accel:���ٶ�
 *				acc_decel:���ٶ�
 *				S_buf:������������
 *@return
 *				1:�ɹ�����send_buf
 *				0:���ù��̳�������
 */
u8 motor_point_movement_ready(float steps, u8 dir, float speed_max, float speed_init, float acc_accel, u16 * S_buf)
{
	if(((u16)(steps+1.5))>send_buf_size) return 0;	/* ���߽� */
	u16 acc_lim = (u16)(0.5+(steps * 0.5));		/* the number of steps before deceleration starts */
	u16 arr_max = (u16)((0.5+(step_angle * timer_frep /speed_max)));
	u16 K = 2*speed_init*speed_init/(acc_accel*step_angle);
	u16 accel_steps = 0;
	u16 *temp = (u16*)malloc(sizeof(u16));
	u16 const_steps = 0;
	u16 i = 0;
	float compensation = 0;
	accel_steps = acc_lim;
		steps = (u16)(0.5+steps);
		while(!(i > steps))
		{
			if(i == 0 && speed_init == 0) *(S_buf + i) = (u16)ceil(timer_frep* sqrt(2*step_angle/acc_accel) * 0.676);
			if(i == 0 && speed_init != 0) *(S_buf + i) = (u16)ceil(timer_frep*(step_angle/speed_init));
			if(i > 0 && i < accel_steps)	/* ���ٶ� */
			{
				*temp = (u16)(0.5+(*(S_buf + i - 1) - ((*(S_buf + i - 1) * 2 + compensation) / (4 * i + 1 + K))));
				if(*temp > arr_max)
					*(S_buf + i) = (*temp);
				else 
				{
					if(const_steps == 0)
					{				
						const_steps = steps - 2*i;
						accel_steps = i;
					}
				}
				compensation = (float)fmod( *(S_buf + i - 1) * 2 + compensation, 4 * i + 1 + K);	/* ���²��� */
			}
			if(i >= accel_steps && i < accel_steps + const_steps)	/* ���ٶ� */
				*(S_buf + i) = arr_max;
			if(i >= accel_steps + const_steps && i < steps)	/* ���ٶ� */
			{
				*(S_buf+i) = (u16)(0.5+(*(S_buf+i-1)+((*(S_buf+i-1)*2+compensation)/(4*i+1+K))));
				compensation = (float)fmod(*(S_buf+i-1)*2+compensation, 4*i+1+K);	/* ���²��� */
			}
			if (i == steps)
			{
				*(S_buf + i) = 0 ;	/* ���� */
				free(temp);
			}
			++i;
		}
		motor_dir(dir);	/* ���õ���˶����� */
		DMA_Cmd(DMA1_Channel6, DISABLE);	/* �޸�DMA����֮ǰ��ȷ��DMA�Ѿ�ʧ�ܣ������޷��޸����� */
		steps>0 ? DMA_SetCurrDataCounter(DMA1_Channel6,(u16)steps + 1) : DMA_SetCurrDataCounter(DMA1_Channel6,0);	/* ��ǰ����DMA�ķ���λ����������ʱ��ʹ��DMA */
		pre_cndtr = (u32)steps;
		return 1;
}

///**
// *@function �õ���˶��켣����send_buf
// *@param
// *				trajectory_buf:�켣��Ϣ������
// *
// *@return void
// */
//void motor_trajectory_config(motion_info * trajectory_buf, float speed_max, float speed_init, u16 * S_buf,u16 offset)
//{
//	float acc_accel = 0;
//	float compensation = 0;
//	u16 steps = (u16)(0.5 + motor_type*Micro_Step*ratio*((trajectory_buf->rad)/pi/2));
//	u16 temp = 0;
//	u16 i = 0;
//	u8 arr = 0;
//	if(trajectory_buf->state != 'G')
//	{
//		if(speed_max != speed_init)	/* �����Ҫ���� */
//		{
//			acc_accel = ((speed_max+speed_init)*(speed_max-speed_init))/ (2*step_angle*steps);
//				if(acc_accel > 0)
//				{
//					while(i < steps)
//					{
//						if(i == 0)
//						{
//							*(S_buf+offset) = (u16)ceil(timer_frep* sqrt(2*step_angle/acc_accel) * 0.676);	/* ��ʼ��send_buf�ĵ�һλ */
//						}else
//						{
//							*(S_buf+offset+i) =(u16)(0.5+(*(S_buf+offset+i-1)-((*(S_buf+offset+i-1)*2+compensation)/(4*(offset+i)-1))));
//							compensation = (float)fmod(*(S_buf+offset+i-1)*2+compensation, 4*(offset+i)-1);	/* ���²��� */
//						}
//						++i;
//					}
//				}else if(acc_accel < 0)
//				{
//					acc_accel = -1*acc_accel;
//					while(i < steps)
//					{
//						if(i == 0)
//						{
//							*(S_buf+offset) = (u16)ceil(timer_frep* sqrt(2*step_angle/acc_accel) * 0.676);	/* ��ʼ��send_buf�ĵ�һλ */
//						}elseu
//						{
//							*(S_buf+offset+i) =(u16)(0.5+(*(S_buf+offset+i-1)-((*(S_buf+offset+i-1)*2+compensation)/(4*(offset+i)-1))));
//							if(i!=1) *(S_buf+offset+i-1) = temp;
//							temp = *(S_buf+offset)*2 - *(S_buf+offset+i);
//							compensation = (float)fmod(*(S_buf+offset+i-1)*2+compensation, 4*(offset+i)-1);	/* ���²��� */
//						}
//						++i;
//					}
//				}
//		}else		/* �������Ҫ���� */
//		{
//			while(i<steps)
//			{
//				if(offset==0)
//				{
//					
//				}
//			}
//		}
//	}
//}

/**
 *@function �����ʼ����
 *@param void
 *@return 
 * 				1:�ɹ�����
 *				0:��δ����
 */
u8 motor_run()
{
	if(Motor_status != m_moving && send_buf[0] != 0 && DMA1_Channel6->CNDTR != 0)	/* ֻ�е���������˶�״̬�����˶���������0ʱ�ſ�ʼ��һ�η��� */
	{
		TIM3->ARR = 2;	/* ����arr��send_buf�����һ����0������������ʱ��ARR�ᱻ���㣬������һ��������Ч���˴�Ӧ�Ƚ�arr�޸�Ϊ����ֵ*/
		DMA_Cmd(DMA1_Channel6, ENABLE);
		TIM_Cmd(TIM3, ENABLE);  /* ʹ��TIM3 */
		TIM3->EGR = 0x00000001;	/* ����TIM3����IO�� */
		Motor_status = m_moving;	/* �л�������״̬ */
		return 1;
	}
	return 0;
}

/**
 *@function ���ֹͣ
 *@param void
 *@return void
 */
void motor_stop()
{
	TIM_Cmd(TIM3,DISABLE);	/* �ȹرն�ʱ�� */
	DMA_Cmd(DMA1_Channel6,DISABLE);	/* �ٹر�DMA */
	DMA1_Channel6->CNDTR = 0;	/* ����CNDTR */
	Motor_status = m_stop;	/* �л����ո�ֹͣ״̬ */
}

/**
 *@function �����λ
 *@param void
 *@return void
 */
void motor_home()
{
	if(home_flag == 0)
	{
		#if SLAVE0||SLAVE1
		motor_point_movement_ready(motor_type*Micro_Step*ratio, 0, 2*pi, 0.1*pi, 0.05, send_buf);
		#endif
		#if SLAVE2
		motor_point_movement_ready(motor_type*Micro_Step*ratio, 1, 2*pi, 0.1*pi, 0.05, send_buf);
		#endif
		#if SLAVE3
		motor_point_movement_ready(motor_type*Micro_Step*ratio*0.6, 0, 10*pi, 0.1*pi, 0.1, send_buf);
		#endif
		motor_run();
	}
	while(home_flag == 0);	/* ����λ����û�б����� */
	/* ��λ���ر����������ֹͣ */
	motor_stop();	/* ֹͣ��� */
	current_position = (-1*home_offset/pi)*motor_type*Micro_Step*ratio;	/* ��ʼ��������̼ƣ�����������̼�ԭ�㣬��ֵ��ʵ�ʻ����˵���λ���ط���λ�þ��� */
	zeroed = 1;	/* ��־Ϊ�Ѿ����� */
	motion_buf_init();	/* ��ʼ���˶���Ϣ������������ԭ�� */
}

/**
 *@function �жϵ��������״̬
 *@param void
 *@return 
 * 				m_moving 	0x01
 * 				m_stop		0x02
 * 				m_waiting 0x03
 */
u8 MotorStatus()
{
	this_cndtr = DMA_send_feedback(DMA1_Channel6);
	current_position = current_position + motion_dir*(pre_cndtr-this_cndtr);	/* ���µ�ǰλ�� */
	pre_cndtr = this_cndtr;
	if(this_cndtr == 0 && Motor_status == m_moving)	/* ���DMA�Ѿ����������ݣ����ҵ����Ȼ��������״̬ */
	{
		Motor_status = m_stop;	/* ���״̬�л���ֹͣ */
		if(zeroed) CAN_send_feedback(c_motor_arrive);	/* �������Ѿ�������궨������֪ͨ���� */
		return m_stop;	/* ��Ϊ����ոյ���ָ��λ�� */
	}
	if(this_cndtr == 0 && (Motor_status == m_stop || Motor_status == m_waiting))
	{
		Motor_status = m_waiting;
		return m_waiting;	/* ��Ϊ������ѵ���ָ��λ�ã����ڵȴ�����״̬ */
	}
	return m_moving;	/* ������Ϊ�����δ����ָ��λ�� */
}

/**
 *@function ��ʼ���˶���Ϣ�������ṹ������ĵ�һλ������ʼ���ؽ��˶���Ϣ������ԭ��
 *@param void
 *@return void 
 */
void motion_buf_init()
{
	motion_buf[0].rad = current_position;
	motion_buf[0].dir = 0;
	motion_buf[0].speed_max = 0;
}
