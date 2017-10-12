#include "stm8s.h"
#include "mystm8s.h"
#include "string.h"
#include "textlcd.h"

/* UART1 TX (PD5) */
/* UART1 RX (PD6) */

/* Global Define */
#define		MS_TEST				"AT"
#define		MS_DELETE_ALL_SMS	"AT+CMGD=1,4"
#define     MS_MODE_TEXT_SMS	"AT+CMGF=1"

#define UART_BUFFER_SIZE		500
#define SMS_BUFFER_SIZE			160

/* Global Variables */
TextLcd_4Bit_HandleTypeDef mylcd;
uint8_t uart_receive_buffer[UART_BUFFER_SIZE];
uint8_t sms_buffer[SMS_BUFFER_SIZE];
uint8_t index=0,count_cr_lf=0,sms_start_index=0,sms_end_index=0,sms_text_index=0,prefix_sms_check=0;
volatile uint8_t new_cmd_from_user=0,state=0;
uint8_t temp=32;

/* Declaring Function Prototype */
void uart_init(void);
void uart_isr(void);
void empty_buffer(uint8_t *buffer,uint16_t size);
void ms_send_cmd(uint8_t * cmd,uint8_t length);
void process_cmd_from_user(void);


void main(void)
{
	/* Configure Clock For STM8 using HSE */
	CLK_CONFIG_16MHZ_HSE();

	/* Configure GPIO for Relay Control */

	/* Relay2 and Relay1 */
	GPIO_Init(GPIOD,(GPIO_Pin_TypeDef)(GPIO_PIN_2|GPIO_PIN_3),GPIO_MODE_OUT_PP_HIGH_FAST);
	/* Relay 3 */
	GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_FAST);

	/* Initialize Timer 4 for Delay function */
	Delay_Using_Timer4_Init();

	/* Initialize Lcd */
	textlcd_init(&mylcd,TextLcd_Dimension16xN,
			GPIOC,GPIO_PIN_3,
			GPIOC,GPIO_PIN_4,
			GPIOC,GPIO_PIN_5,
			GPIOC,GPIO_PIN_6,
			GPIOC,GPIO_PIN_7,
			GPIOB,GPIO_PIN_4);

	textlcd_puts(&mylcd,0,0,(int8_t*)"Module SIM Dang");
	textlcd_puts(&mylcd,1,0,(int8_t*)"Khoi Dong...");

	/* Initialize Uart */
	uart_init();

	/* Wait Module SIM Start */
	Delay_ms(3000);

	/* Send Command To Module Sim */
	ms_send_cmd(MS_TEST,strlen((const char *)MS_TEST));
	Delay_ms(200);
	ms_send_cmd(MS_DELETE_ALL_SMS,strlen((const char*)MS_DELETE_ALL_SMS));
	Delay_ms(200);
	ms_send_cmd(MS_MODE_TEXT_SMS,strlen((const char *)MS_MODE_TEXT_SMS));
	Delay_ms(200);
	empty_buffer(uart_receive_buffer,UART_BUFFER_SIZE);

	textlcd_clear(&mylcd);

	textlcd_puts(&mylcd,0,0,(int8_t*)"TB1");
	textlcd_puts(&mylcd,0,6,(int8_t*)"TB2");
	textlcd_puts(&mylcd,0,12,(int8_t*)"TB3");

	textlcd_puts(&mylcd,1,0,(int8_t*)"OFF");
	textlcd_puts(&mylcd,1,6,(int8_t*)"OFF");
	textlcd_puts(&mylcd,1,12,(int8_t*)"OFF");

	while (1)
	{
		process_cmd_from_user();
	}
}

void process_cmd_from_user(void)
{
	if(new_cmd_from_user)
	{
		new_cmd_from_user=0;
		if((sms_buffer[11]=='O')&&(sms_buffer[12]=='N'))
		{
			state=1;
		}
		else if((sms_buffer[11]=='O')&&(sms_buffer[12]=='F')&&(sms_buffer[13]=='F'))
		{
			state=0;
		}
		if((sms_buffer[8]=='T')&&(sms_buffer[9]=='B'))
		{
			switch(sms_buffer[10])
			{
			case '1':
				if(state)
				{
					GPIO_WriteLow(GPIOD,GPIO_PIN_3);
					textlcd_puts(&mylcd,1,0,(int8_t*)"ON ");
				}
				else
				{
					GPIO_WriteHigh(GPIOD,GPIO_PIN_3);
					textlcd_puts(&mylcd,1,0,(int8_t*)"OFF");
				}
				break;
			case '2':
				if(state)
				{
					GPIO_WriteLow(GPIOD,GPIO_PIN_2);
					textlcd_puts(&mylcd,1,6,(int8_t*)"ON ");
				}
				else
				{
					GPIO_WriteHigh(GPIOD,GPIO_PIN_2);
					textlcd_puts(&mylcd,1,6,(int8_t*)"OFF");
				}
				break;
			case '3':
				if(state)
				{
					GPIO_WriteLow(GPIOB,GPIO_PIN_5);
					textlcd_puts(&mylcd,1,12,(int8_t*)"ON ");
				}
				else
				{
					GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
					textlcd_puts(&mylcd,1,12,(int8_t*)"OFF");
				}
				break;
			}
		}
		empty_buffer(sms_buffer,SMS_BUFFER_SIZE);
	}
}
void uart_isr(void)
{
	if(UART1_GetITStatus(UART1_IT_RXNE))
	{
		uart_receive_buffer[index]=UART1_ReceiveData8();
		if(index>2)
		{
			/* Check Key Word Of SMS */
			if((uart_receive_buffer[index-3]=='+')&&(uart_receive_buffer[index-2]=='C')&&(uart_receive_buffer[index-1]=='M')&&(uart_receive_buffer[index]=='T'))
			{
				prefix_sms_check=1;
			}
			if(prefix_sms_check)
			{
				/* Check the end of respond from module sim */
				if((uart_receive_buffer[index]=='\n')&&(uart_receive_buffer[index-1]=='\r'))
				{
					count_cr_lf++;
					if(count_cr_lf==1)
					{
						sms_start_index=index+1;
					}
					if(count_cr_lf==2)
					{
						count_cr_lf=0;
						sms_end_index=index-1;

						/* Copy new message to sms_buffer */
						for(uint8_t i=sms_start_index;i<sms_end_index;i++)
						{
							sms_buffer[sms_text_index]=uart_receive_buffer[i];
							sms_text_index++;
						}

						/* Check User Password To Control Device */
						if((sms_buffer[0]=='s')&&(sms_buffer[1]=='u')&&(sms_buffer[2]=='b')&&(sms_buffer[3]=='z')&&(sms_buffer[4]=='e')&&(sms_buffer[5]=='r')&&(sms_buffer[6]=='o')&&(sms_buffer[7]=='s'))
						{
							new_cmd_from_user=1;
						}
						sms_start_index=0;
						sms_end_index=0;
						sms_text_index=0;
						prefix_sms_check=0;
						empty_buffer(uart_receive_buffer,UART_BUFFER_SIZE);
						return;
					}
				}
			}
			else if(index>300)
			{
				empty_buffer(uart_receive_buffer,UART_BUFFER_SIZE);
			}
		}
		index++;
	}
}

void ms_send_cmd(uint8_t * cmd,uint8_t length)
{
	for(uint8_t i=0;i<length;i++)
	{
		while(!UART1_GetFlagStatus(UART1_FLAG_TC));
		UART1_SendData8(*(cmd+i));
	}

	/* Send CR (Carriage Return) */
	while(!UART1_GetFlagStatus(UART1_FLAG_TC));
	UART1_SendData8(0x0D);
}

void empty_buffer(uint8_t *buffer,uint16_t size)
{
	for(uint16_t i=0;i<size;i++)
	{
		*(buffer+i)=0;
	}

	index=0;
}

void uart_init(void)
{
	/* Initialize uart1 mode 8bit data,
	 * 1bit stop, no parity, buad rate=115200
	 */
	UART1_Init(115200,
			UART1_WORDLENGTH_8D,
			UART1_STOPBITS_1,
			UART1_PARITY_NO,
			UART1_SYNCMODE_CLOCK_DISABLE,
			UART1_MODE_TXRX_ENABLE);

	/* Enable uart1 receive complete interrupt */
	UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);

	/* Enable uart1 module */
	UART1_Cmd(ENABLE);
	enableInterrupts();
}










#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval : None
 */
void assert_failed(u8* file, u32 line)
{ 
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
