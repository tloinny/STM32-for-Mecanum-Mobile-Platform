/**
 *@title Abot Firmware
 * Copyright: Copyright (c) 2019 Abot [https://github.com/tloinny/STM32-for-Abot]
 *
 *@created on 2019-1-08  
 *@author:tony-lin
 *@version 1.0.0 
 * 
 *@description: DMA�ײ�����
 */

#include "dma.h"

DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;	 /* ����DMAÿ�����ݴ��͵ĳ��� */

/**
 *@function DMA1�ĸ�ͨ������
 *����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
 *�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
 *@param 
 *			DMA_CHx:DMAͨ��CHx
 *			cpar:�����ַ
 *			cmar:�洢����ַ
 *			cndtr:���ݴ�����
 *@return void
 */
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	/* ʹ��DMA��Դ */
	delay_ms(5);
	
  DMA_DeInit(DMA_CHx);   /* ��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ */

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  /* DMA�������ַ */
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  /* DMA�ڴ����ַ */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  /* ���ݴ��䷽�򣬴��ڴ��ȡ���͵����� */
	DMA_InitStructure.DMA_BufferSize = cndtr;  /* DMAͨ����DMA����Ĵ�С */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  /* �����ַ�Ĵ������� */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  /* �ڴ��ַ�Ĵ������� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  /* ���ݿ��Ϊ16λ */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; /* ���ݿ��Ϊ16λ */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  /* ����������ģʽ */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; /* DMAͨ�� xӵ�������ȼ� */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  /* DMAͨ��xû������Ϊ�ڴ浽�ڴ洫�� */
	DMA_Init(DMA_CHx, &DMA_InitStructure);  	
} 

/**
 *@function ���ȷ���������ʣ�µ�������
 *@param 
 *			DMA_CHx:DMAͨ��CHx
 *@return void
 */
u16 DMA_send_feedback(DMA_Channel_TypeDef* DMA_CHx)
{
	return DMA_CHx->CNDTR;
}  

