/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

extern char g_DatRev[Max_UART_String];
extern u8 g_DatRev_num,UART_FLAG;
extern u16 g_DatRev_num_uart2; //usart2��������
extern u8 Display_PWM[MATRIX_MODULE*MATRIX_HEIGHT*2][3];
extern u8 Control;
//unsigned char Trg; 
//unsigned char Cont; 
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/* I/O���жϣ��ж���ΪPC13 */
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line13) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  {
      while(!(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)));
				GPIO_WriteBit(GPIOC, GPIO_Pin_9, 
						(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9))));
			if(Control<120)
				Control++;
			else
				Control=0;
			Control |= 0x80; //λ�ø�
//		  unsigned char ReadData = 0xfe ^ 0xff;
//		  Trg = ReadData & (ReadData ^ Cont);
//		  Cont = ReadData;
      EXTI_ClearITPendingBit(EXTI_Line13);     //����жϱ�־λ
  }
}


/**
  * @}
  */ 
void USART1_IRQHandler(void) 
{
	  u8 Cache_num;
		if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�ж��Ƿ��������ж�
		{
  			//GPIO_ResetBits(GPIOC, GPIO_Pin_12); //LED_RED
        g_DatRev[g_DatRev_num]=USART_ReceiveData(USART1);   //�������ݣ�
			  Cache_num=USART_ReceiveData(USART1);  //����Ƚϻ�����
//				USART_SendData(USART1,g_DatRev[g_DatRev_num]);
			  if(Cache_num==0x0A | g_DatRev_num>=(Max_UART_String-1))  //�жϵ����յ�'\n' ���߳�������ʱ
				{
						if(Cache_num==0x0A)  //����ǽ��ܵ�'\n'��־λ��һλ��1
						{
								UART_FLAG |= 0x01;
 //               USART_SendData(USART1,'Z');
						}
						else                 //������ǣ���˵�������ְλ����2λ��1
								UART_FLAG |= 0x02;
        }
				else
            g_DatRev_num++;   //��¼����
//				while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}//�ȴ����ܽ���
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);              //����жϱ�־    
		}
		USART_ClearFlag(USART1,USART_FLAG_TC);                     //������ɱ�־λ
}

void USART2_IRQHandler(void) 
{
		if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//�ж��Ƿ��������ж�
		{
			//USART_SendData(USART2,USART_ReceiveData(USART2));	//���ط�������
			//	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}//�ȴ����ܽ���
			Display_PWM[g_DatRev_num_uart2/3][g_DatRev_num_uart2-((g_DatRev_num_uart2/3)*3)]=USART_ReceiveData(USART2);   //�������ݣ�
			if(g_DatRev_num_uart2<3072)
				g_DatRev_num_uart2++;
			else
				g_DatRev_num_uart2=1;
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);              //����жϱ�־    
		}
		USART_ClearFlag(USART2,USART_FLAG_TC);                     //������ɱ�־λ
}

/*
 * ��ʱ��2�жϷ���
 * �жϺ������� ��ʱ��λ 200ms һ�δ�����������λ����
 */
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);    
  	//�жϷ���
		Timer_Move_interrupt();
	}
}

/**
  * @brief  ��ʱ��3 ��ʾ�жϺ���
  * @param  None
  * @retval : None
  */
void TIM3_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
		display_PWM(); //��ʾ����
	}
}

/**
  * @brief  ��ʱ��4 ��ʱ����
  * @param  None
  * @retval : None
  */
void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
		delay_interupt(); //��ʾ����
	}
}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Toggle GPIO_LED pin 6 each 1s */
    //GPIO_WriteBit(GPIO_LED, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_LED, GPIO_Pin_6)));

    /* Enable time update */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC_GetCounter() == 0x0001517F)
    {
      RTC_SetCounter(0x0);
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
    }
  }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
