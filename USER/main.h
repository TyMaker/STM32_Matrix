/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    04-April-2016
  * @brief   main ����ͷ�ļ�
  *
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define	__MAIN_H

#include <string.h>

#include "stm32f10x.h"
#include "stm32f10x_rgb_systick.h"
#include "stm32f10x_rgb_uart.h"
#include "stm32f10x_rgb_err.h"
#include "stm32f10x_rgb_init.h"
#include "stm32f10x_rgb_matrix.h"
#include "stm32f10x_rgb_spi_flash.h"
#include "stm32f10x_rgb_rtc.h"
#include "stm32f10x_rgb_i2c.h"
#include "stm32f10x_rgb_dht11.h"
#include "stm32f10x_rgb_gfx.h"
#include "stm32f10x_rgb_Colors.h"
#include "stm32f10x_rgb_Appliction.h"
#include "stm32f10x_rgb_adc.h"
#include "stm32f10x_rgb_Types.h"

/****************************  SPI  **************************/
/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)  //��ȡ�����С
#define  Read_BufferSize 128



/**************** I2C ****************************/
#define EEP_Firstpage      0x00   //i2c ��ַ
#define EEP_NUMBER 3              //�ֽ���

extern u8 hour,min,sec,w_month,w_date,week;//Сʱ�����ӣ��룬ȫ�ֱ���
extern u16 w_year;
extern u8 ScrollPosition; //��λ������
extern u32 Timing;
extern bitmap_font *font;

extern u8 Control;
DHT11_Data_TypeDef DHT11_Data;    //�¶�ʪ�Ȼ���

void System_Init(void);
void Text(void);
void Move(void);

#endif /* __MAIIN_H */

/******************* (C) COPYRIGHT 2011 Makt *****END OF FILE****/
