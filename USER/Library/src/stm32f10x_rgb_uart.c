/**
  ******************************************************************************
  * @file    stm32f10x_rgb_uart.c
  * @author  Makt
  * @version V1.0
  * @date    2016-04-04
  * @brief   串口功能文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "stm32f10x_rgb_uart.h"
#include <stdarg.h>

char g_DatRev[Max_UART_String] = {0};  //USART缓冲
u8 g_DatRev_num=0,UART_FLAG=0;  //计数 标志位  UART_FLAG = 0x01 为接受到字符串 0x02 为接受溢出
//u16 g_DatRev_num_uart2;   //usart2 计数缓存
//u8 aaa;
/*
 * 函数名：UART1_Config
 * 描述  ：UART1_Config 配置，工作模式配置  115200-8-N-1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void USART1_Config(u8 EN,u32 Baud_Rate) // 115200 65535
{
	if(EN==ENABLE){
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure); 
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART1, ENABLE);
		USART_ClearFlag(USART1,USART_FLAG_TC);                     //发送完成标志位
  }
}

void USART2_Config(u8 EN,u32 bound){
	if(EN==ENABLE){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
		
			/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
 
	USART_DeInit(USART2);  
  //USART2_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  //USART2_RX     PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????????
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //??????PA10
 
   
  //USART ???????�??
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
 
		USART_Init(USART2, &USART_InitStructure); 
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, ENABLE);  
	}		
}

char strcmp1(char *lk,char *ls)
{
		char *a = lk, *b = ls;
		u8 bSame = TRUE;
		while (*b!='\0')
		{
				if (*a++ != *b++) 
				{
						bSame = FALSE; break;
				}
		}
    return bSame;
}

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
    /* 将printf内容发往串口 */
	  USART_SendData(USART1, (unsigned char) ch);
	  while(!(USART1->SR & USART_FLAG_TXE));
	
	  return (ch);
}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */
/*
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{
		if ( *Data == 0x5c )  //'\'
		{
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}

void Get_Flag()
{
		if(UART_FLAG==0x01)   //如果标志位第一位置一说明发生接收到'\n'
		{
				UART_FLAG &= 0xFE;  //清除标志位
				printf("String is:%s\n",g_DatRev);
				//当接受到数据后，进入这个函数，分析处理
				
				if(strcmp1(g_DatRev,"AT+RST\r\n"))
				{
					printf("OK\r");
					SystemReset();
				}
				else if(strcmp1(g_DatRev,"AT+SPI_FLASH_BulkErase\n"))
				{
						//SPI_FLASH_BulkErase();
						printf("[OK] SPI_FLASH_BulkErase OK");
				}
				else if(strcmp1(g_DatRev,"AT+W\r\n"))
				{
					  //aaa = 'w';
				}
				else if(strcmp1(g_DatRev,"AT+S\r\n"))
				{
					  //aaa = 's';
				}
				else if(strcmp1(g_DatRev,"AT+A\r\n"))
				{
					  //aaa = 'a';
				}
				else if(strcmp1(g_DatRev,"AT+D\r\n"))
				{
					  //aaa = 'd';
				}
				else if(strcmp1(g_DatRev,"AT+CLOSE\r\n"))
				{
						printf("[OK] CLOSE SYS\n");
				}

				//if(strcmp1(g_DatRev,"AT+HELP\r\n"))
				else{
						printf("***   System Help   ***\n");
						printf("Please any AT+ ..  \n");
						printf("AT+RST to reboot System\n");
						printf("AT+SPI_FLASH_BulkErase to Erase\n");
						printf("AT+CLOSE close led\n");
				}
				/*    ##################    */
				while(g_DatRev_num)  //清空接受缓冲器
				{
						 g_DatRev[g_DatRev_num]=0;
						 g_DatRev_num--;
				}
		}
		else if(UART_FLAG==0x02)  //如果发生了溢出 
		{
				UART_FLAG &= 0xFD;  //清除标志位
				printf("\r\n Data overflow \r\n");
				while(g_DatRev_num)    //清空接受缓冲
				{
						 g_DatRev[g_DatRev_num]=0;
						 g_DatRev_num--;
				}
		}
}

/*******************************************************************************
* Function Name  : SystemReset
* Description    : Configures the port pin connected to the push button. GPIO_D_4
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__asm void SystemReset(void)
{
 MOV R0, #1           //;
 MSR FAULTMASK, R0    //;
 LDR R0, =0xE000ED0C  //;
 LDR R1, =0x05FA0004  //;
 STR R1, [R0]         //;
 
deadloop
    B deadloop        //;
}

/************************* (C) COPYRIGHT 2016  END OF FILE **********************/ 
