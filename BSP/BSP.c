/*
********************************************************************************
*                                  uC/OS-III
*							
*                              ARM Cortex-M3 Port
*
* File      	: Config.C
* Version   	: V1.0
* By        	: ����ǿ
*
* For       	: Stm32f10x
* Mode      	: Thumb2
* Toolchain 	: 
*             		RealView Microcontroller Development Kit (MDK)
*             		Keil uVision
* Description   : STM32F10x �ڲ� ϵͳ������
*
*					1��ϵͳ�ж����ȼ�ģʽ����
*					2��ϵͳ��������ָ��
*					3��ϵͳʱ�Ӽ�ʱ������
*					4��оƬ���ų�ʼ��
*					
* Date          : 2012.05.22
*******************************************************************************/

#include "includes.h"
#include "stm32f10x_conf.h"
#include "bsp.h"




/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
#ifdef USE_STM3210B_EVAL
	/* Enable the USART2 Pins Software Remapping */
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                     RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                     RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
	                     RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
	                     RCC_APB2Periph_GPIOE, DISABLE);  

}


/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(volatile CPU_INT32U nCount)
{
  for(; nCount != 0; nCount--);
}

/*******************************************************************************
�� �� ����void IWDG_Init(void)
�������������Ź���ʼ��						
��ڲ�����							
���ز�����
����ʱ��: 2011.6.24
********************************************************************************/
void IWDG_Init(void)
{
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( IWDG_Prescaler_64);	//��С
	IWDG_SetReload( 0x138);		//40KHz�ڲ�ʱ�� (1/40000 * 64 * 0x138 = 0.5s)
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Disable );
	IWDG_Enable();
	IWDG_ReloadCounter();
}

/*******************************************************************************
* Function Name :void SysTickInit(void)
* Description   :ϵͳ��ʱ��ʱ������
* Input         :
* Output        :
* Other         :ʱ��Ϊ1ms
* Date          :2011.11.03  12:59:13
*******************************************************************************/
void SysTickInit(void)
{
	SysTick_Config(SystemCoreClock / 1000);			//uCOSʱ��1ms
}

/*******************************************************************************
* Function Name :void InterruptOrder(void)
* Description   :�ж����������ȼ�
* Input         :
* Output        :
* Other         :
* Date          :2011.10.27  11:50:05
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4);//���ȼ�����  ȫΪ��ռʽ���ȼ�
}


/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :ϵͳʱ��� ��ʼ��
* Input         :
* Output        :
* Other         :
* Date          :2012.6.15  13:14:59
*******************************************************************************/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
}
#endif


/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :��ʱ��� ����ֵ
* Input         :�����ļ���ֵ
* Output        :
* Other         :
* Date          :2012.6.15  13:14:59
*******************************************************************************/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return (SysTick->VAL);
}
#endif

/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :ϵͳ��ʼ��
* Input         :
* Output        :
* Other         :
* Date          :2011.10.27  13:14:59
*******************************************************************************/
void BspInit(void)
{
	NVIC_Configuration();	//�ж����ȼ�����
	GPIO_Configuration();	//�˿ڳ�ʼ�������ж˿ڹ�
}

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}



void led_on(CPU_INT32U n)
{
	switch (n)
	{
		case LED_0:
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
		break;
		case LED_1:
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
		break;
		case LED_2:
		GPIO_SetBits(GPIOF, GPIO_Pin_6);
		break;
		case LED_3:
		GPIO_SetBits(GPIOF, GPIO_Pin_7);
		break;
		case LED_4:
		GPIO_SetBits(GPIOF, GPIO_Pin_8);
		break;
		case LED_5:
		GPIO_SetBits(GPIOF, GPIO_Pin_9);
		break;
		default:
		break;
	}
}


void led_off(CPU_INT32U n)
{
	switch (n)
	{
		case LED_0:
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
		break;
		case LED_1:
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		break;
		case LED_2:
		GPIO_ResetBits(GPIOF, GPIO_Pin_6);
		break;
		case LED_3:
		GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		break;
		case LED_4:
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		break;
		case LED_5:
		GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		break;
		default:
		break;
	}
}


/* USART functions -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
  
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  return ch;
}

void usart1_init(CPU_INT32U BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  /* Initialize Clock for COM port(USART)*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
  
  USART_DeInit(USART1);                                 //��ʼ������1
  
  /* Initialize COM port(USART) available on*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;             //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       //�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //��ʼ�� PA9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //��ʼ�� PA10


  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = BaudRate;        //BaudRate usually 115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART1,&USART_InitStructure);
  
#ifdef  EN_USART1_RX 
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //��ռ0-3 ��Ӧ0-3

  /* Initialize Interrupt for COM port(USART)*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;      //��ռ���ȼ� 3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;            //�����ȼ� 3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //IRQ ͨ��ʹ��
  
  NVIC_Init(&NVIC_InitStructure);                               //����ָ���Ĳ�����ʼ�� NVIC �Ĵ���  
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                //�����ж�
#endif /* EN_USART1_RX */
  USART_Cmd(USART1, ENABLE);
}


/*
T_CS PA4
SPI1_SCK PA5
SPI1_MISO PA6
SPI1_MOSI PA7
T_BUSY PA8
*/
void SPI_Config(void) 
{ 
    GPIO_InitTypeDef  GPIO_InitStructure; 
    SPI_InitTypeDef   SPI_InitStructure; 
    
    //GPIOA Periph clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
    //SPI1 Periph clock enable 
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
    //SPI2 Periph clock enable 
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE ) ;
    
    //Configure SPI2 pins: SCK, MISO and MOSI 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    GPIO_Init(GPIOB,&GPIO_InitStructure);  
    /*
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
    GPIO_Init(GPIOA,&GPIO_InitStructure);     
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
    GPIO_Init(GPIOA,&GPIO_InitStructure);     */
    
    //Configure PF10 pin: TP_CS pin 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //�������
    GPIO_Init(GPIOB,&GPIO_InitStructure); 
    
    //Configure PA8 pin: TP_BUSY pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //��������
    GPIO_Init(GPIOE,&GPIO_InitStructure); 
    
    /* Configure PE.06 as input floating For TP_IRQ*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
    
    // SPI1 Config  
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //SPI_NSS_Hard
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
    SPI_InitStructure.SPI_CRCPolynomial = 7; 
    SPI_Init(SPI2,&SPI_InitStructure); 
    
    // SPI1 enable  
    SPI_Cmd(SPI2,ENABLE);  
}


unsigned char SPI_WriteByte(unsigned char data) 
{ 
    unsigned char Data = 0; 
    
    //Wait until the transmit buffer is empty 
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET); 
    // Send the byte  
    SPI_I2S_SendData(SPI2,data); 
    
    //Wait until a data is received 
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET); 
    // Get the received data 
    Data = SPI_I2S_ReceiveData(SPI2); 
    
    // Return the shifted data 
    return Data; 
}  
void SpiDelay(unsigned int DelayCnt)
{
     unsigned int i;
     for(i=0;i<DelayCnt;i++);
}

u16 TPReadX(void)
{ 
    u16 x=0;
    TP_CS();
    SpiDelay(10);
    SPI_WriteByte(0x90);
    SpiDelay(10);
    x=SPI_WriteByte(0x00);
    x<<=8;
    x+=SPI_WriteByte(0x00);
    SpiDelay(10);
    TP_DCS(); 
    x = x>>3;
    return (x);
}

u16 TPReadY(void)
{
    u16 y=0;
    TP_CS();
    SpiDelay(10);
    SPI_WriteByte(0xD0);
    SpiDelay(10);
    y=SPI_WriteByte(0x00);
    y<<=8;
    y+=SPI_WriteByte(0x00);
    SpiDelay(10);
    TP_DCS();
    y = y>>3; 
    return (y);
}


