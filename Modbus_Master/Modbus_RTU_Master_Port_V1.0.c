/*
******************************************************************************************************************
*Filename      :Modbus_RTU_Slaver_Core.c
*Programmer(s) :Nrush
*Description   :该文件为Modbus RTU从机通讯核心代码与硬件平台无关，缺乏字节超时校验，其余功能完整，
******************************************************************************************************************
*/
/* $ PAGE*/
#include "Modbus_RTU_Slaver_port.h"		//对外接口，生成LIB文件后必须包含该文件
#include "sys.h"	
#include "stdio.h"
/*
******************************************************************************************************************
*                                           CONSTANTS & VARIABLE
*Description : 变量定义
*Notes       : none
******************************************************************************************************************
*/

/******************************************************************************************************************
*                                   Modbus_Usart_Init_Port(uint8 usart_reg)
*Description : 初始化串口接口函数
*Arguments   : usart_reg 串口初始化所用参数
*Returns     : None
*Notes       :	移植该函数时需要注意以下几点(x为usart_reg参数)：	
				（1）调用MODBUS_READ_BOUND(x)获得串口波特率，其返回值为形如
					MODBUS_USART_BAUDRATE_xxxx的宏，代表不同波特率，依据此信息
					对串口波特率进行初始化。
				（2）调用MODBUS_READ_PARITY(x)获得串口校验方法，其返值为
						MODBUS_USART_PARITY_NO：无校验
						MODBUS_USART_PARITY_EVEN：偶校验
						MODBUS_USART_PARITY_ODD：奇校验
					依据此信息对串口校验方式进行初始化。需要注意的是无校验时
					停止位应该为两位，有校验时停止位为1位。
				（3）调用MODBUS_READ_USARTID(x)获得串口号，其返值为MODBUS_USARTx
					共四个串口可实现，依据此信息选择初始化哪一个串口。
				（4）若需要在线调整串口参数，可调用MODBUS_WRITE_USARTID(reg,data)
					MODBUS_WRITE_PARITY(reg,data)，MODBUS_WRITE_BOUND(reg,data)对
					其参数进行需改，调用Modbus_Refresh_Set(modbus_st* modbus)后生效。
				（5）若您不需要在程序运行过程中对串口参数进行修改，可直接固定初始化
					串口的参数，无须理会usart_reg包含的信息。如需要对部分参数进行修改
					忽略不需要修改的参数即可。
				（6）请合理设置串口接收中断优先级，避免串口无法正常接收数据！
				
*******************************************************************************************************************
*/
void Modbus_Usart_Init_Port(uint8 usart_reg)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint8 temp; 
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	//USART1_TX   PA.9
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
   GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
   //USART1_RX	  PA.10
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
   GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
   temp  = MODBUS_READ_BOUND(usart_reg);
   
   switch(temp)
   {
      case MODBUS_USART_BAUDRATE_2400: USART_InitStructure.USART_BaudRate = 2400;break;
		case MODBUS_USART_BAUDRATE_4800: USART_InitStructure.USART_BaudRate = 4800;break;
		case MODBUS_USART_BAUDRATE_9600: USART_InitStructure.USART_BaudRate = 9600;break;
		case MODBUS_USART_BAUDRATE_14400:   USART_InitStructure.USART_BaudRate = 14400;break;
		case MODBUS_USART_BAUDRATE_19200:   USART_InitStructure.USART_BaudRate = 19200;break;
		case MODBUS_USART_BAUDRATE_28800:   USART_InitStructure.USART_BaudRate = 28800;break;
		case MODBUS_USART_BAUDRATE_38400:   USART_InitStructure.USART_BaudRate = 38400;break;
		case MODBUS_USART_BAUDRATE_57600:   USART_InitStructure.USART_BaudRate = 57600;break;
		case MODBUS_USART_BAUDRATE_115200:  USART_InitStructure.USART_BaudRate = 115200;break;
		default:USART_InitStructure.USART_BaudRate = 9600;break;
   }
   
   temp = MODBUS_READ_PARITY(usart_reg);
   
    switch(temp)
   {
      case MODBUS_USART_PARITY_NO:  USART_InitStructure.USART_StopBits = USART_StopBits_2;
                                    USART_InitStructure.USART_Parity = USART_Parity_No;
                                    USART_InitStructure.USART_WordLength = USART_WordLength_8b;break;
		case MODBUS_USART_PARITY_EVEN:USART_InitStructure.USART_StopBits = USART_StopBits_1;
                                    USART_InitStructure.USART_Parity = USART_Parity_Even;
                                    USART_InitStructure.USART_WordLength = USART_WordLength_9b;break;
		case MODBUS_USART_PARITY_ODD:USART_InitStructure.USART_StopBits = USART_StopBits_1;
                                    USART_InitStructure.USART_Parity = USART_Parity_Odd;
                                    USART_InitStructure.USART_WordLength = USART_WordLength_9b;break;
		default:USART_InitStructure.USART_StopBits = USART_StopBits_2;
                                    USART_InitStructure.USART_Parity = USART_Parity_No;
                                    USART_InitStructure.USART_WordLength = USART_WordLength_8b;break;
   }
   
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
   USART_Init(USART1, &USART_InitStructure); 
   USART_ClearITPendingBit(USART1,USART_IT_RXNE);
   USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
   USART_Cmd(USART1, ENABLE);                    
}
/******************************************************************************************************************
*                                   void USART1_IRQHandler(void)
*Description : 串口中断函数
*Arguments   : None
*Returns     : None
*Notes       :	需要在此接收中断函数中调入Modbus_Receive_Data_IRQ(&modbus,Res)
				Res为接收到的字符。				
*******************************************************************************************************************
*/
void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	  u8 Res;
  	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		 {
    
	     	Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
          Modbus_Receive_Data_IRQ(&modbus,Res);
		 }   		 
 }
/******************************************************************************************************************
*                            Modbus_Usart_IRQ_Disable_Port(uint8 usart_reg)
*Description : 串口接收中断使能函数
*Arguments   : usart_reg 串口参数信息
*Returns     : None
*Notes       : 保证失能后串口接收中断标志被清除。				
*******************************************************************************************************************
*/
void Modbus_Usart_IRQ_Disable_Port(uint8 usart_reg)	
{
   USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
   USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}
/******************************************************************************************************************
*                            Modbus_Usart_IRQ_Enable_Port(uint8 usart_reg)
*Description : 串口接收中断使能函数
*Arguments   : usart_reg 串口参数信息
*Returns     : None
*Notes       :	请保证使能前串口接收中断标志被清除。				
*******************************************************************************************************************
*/
void Modbus_Usart_IRQ_Enable_Port(uint8 usart_reg)		/*用来初始化串口的函数*/
{	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}
/******************************************************************************************************************
*                            Modbus_Send_Byte_Port(uint8 usart_reg)
*Description : 串口发送一字节接口函数
*Arguments   : dat 待发送信息
*Returns     : None
*Notes       : None				
*******************************************************************************************************************
*/
void Modbus_Send_Byte_Port(uint8 dat)					/*用来重置并开启定时器及其中断的函数*/
{
    while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) dat;	
}
/******************************************************************************************************************
*                            Modbus_Timer_Iint_Port(uint8 usart_reg)
*Description : 定时器初始化接口函数
*Arguments   : tick_kHz 定时器计数频率，单位为kHz
*Returns     : None
*Notes       : 注意该参数并不是指定时器时钟频率，而是指定时器计数的频率。
*******************************************************************************************************************
*/
void Modbus_Timer_Iint_Port(uint16 tick_kHz)			/*用来初始化定时器的函数*/
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 50000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =((72000/tick_kHz)-1); //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
   
   TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
   TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM2, DISABLE);  //使能TIMx	
}
/******************************************************************************************************************
*                           TIM2_IRQHandler(void)
*Description : 串口中断函数
*Arguments   : None
*Returns     : None
*Notes       : 在该中断中需要对超时标志位累加
*******************************************************************************************************************
*/
void TIM2_IRQHandler(void)
{
 	 if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) 
	{
       modbus.timer.timeout_flag++;
	}
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 
}

/******************************************************************************************************************
*                          void Modbus_Timer_IRQ_Time_Set_Port(uint16 ticks)
*Description : 定时器溢出设置接口函数
*Arguments   : ticks 溢出节拍数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Timer_IRQ_Time_Set_Port(uint16 ticks)		/*用来设定定时器中断时间的函数*/
{
	TIM2->ARR = ticks;
}
/******************************************************************************************************************
*                            void Modbus_Timer_Clear_Port(void)
*Description : 停止计数器、清除定时器计数值、失能中断及清除中断标志位接口函数
*Arguments   : None
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Timer_Clear_Port(void)						/*用来重置并停止定时器及其中断的函数*/
{
	TIM_Cmd(TIM2, DISABLE);
   TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE );
   TIM_SetCounter(TIM2,0);
   TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

/******************************************************************************************************************
*                            void Modbus_Timer_Start_Port(void)
*Description : 开启定时器计数、使能中断接口函数
*Arguments   : None
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Timer_Start_Port(void)						/*用来重置并开启定时器及其中断的函数*/
{
	TIM_SetCounter(TIM2,0);
   TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
   TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );
   TIM_Cmd(TIM2, ENABLE);
}
/******************************************************************************************************************
*                           void Modbus_Save_Byte_Port(uint32 add,uint8 data)
*Description : 保存一字节至存储器
*Arguments   : add 储存地址，data 数据
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Save_Byte_Port(uint32 add,uint8 data)		
{
	
}
/******************************************************************************************************************
*                            uint8 Modbus_Load_Byte_Port(uint32 add)
*Description : 从存储器读取一字节
*Arguments   : add 储存地址
*Returns     : 读得的数据值
*Notes       : None
*******************************************************************************************************************
*/
uint8 Modbus_Load_Byte_Port(uint32 add)				/*用来重置并开启定时器及其中断的函数*/
{	
	return 0;
}
/******************************************************************************************************************
*                            Modbus_Timer_Iint_Port(uint8 usart_reg)
*Description : 定时器初始化接口函数
*Arguments   : tick_kHz 定时器计数频率，单位为kHz
*Returns     : None
*Notes       : 注意该参数并不是指定时器时钟频率，而是指定时器计数的频率。
*******************************************************************************************************************
*/
uint8 holdbit[4] = {0xf5,0xa5,0xff,0x00};

boolean Modbus_Read_InBit_Port(uint16 add,boolean* buf)
{
     if(holdbit[add/8]&(0x01<<(add%8)))
     {
        (*buf) = 1;
     }
      else
     { 
        (*buf) = 0;
     }   
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            Modbus_Timer_Iint_Port(uint8 usart_reg)
*Description : 定时器初始化接口函数
*Arguments   : tick_kHz 定时器计数频率，单位为kHz
*Returns     : None
*Notes       : 注意该参数并不是指定时器时钟频率，而是指定时器计数的频率。
*******************************************************************************************************************
*/
boolean Modbus_Write_HoldBit_Port(uint16 add,boolean dat)
{
   holdbit[add/8] = (~(0x01<<(add%8)))& holdbit[add/8];
   holdbit[add/8] = holdbit[add/8] | (dat<<(add%8));
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            Modbus_Timer_Iint_Port(uint8 usart_reg)
*Description : 定时器初始化接口函数
*Arguments   : tick_kHz 定时器计数频率，单位为kHz
*Returns     : None
*Notes       : 注意该参数并不是指定时器时钟频率，而是指定时器计数的频率。
*******************************************************************************************************************
*/
boolean Modbus_Read_HoldBit_Port(uint16 add,boolean* buf)
{
     if(holdbit[add/8]&(0x01<<(add%8)))
     {
        (*buf) = 1;
     }
      else
     { 
        (*buf) = 0;
     }   
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            Modbus_Timer_Iint_Port(uint8 usart_reg)
*Description : 定时器初始化接口函数
*Arguments   : tick_kHz 定时器计数频率，单位为kHz
*Returns     : None
*Notes       : 注意该参数并不是指定时器时钟频率，而是指定时器计数的频率。
*******************************************************************************************************************
*/
boolean Modbus_Read_In2Bytes_Port(uint16 add,uint16* buf)
{
   (*buf) = ((uint16)holdbit[add*2]<<8)|((uint16)holdbit[add*2+1]);
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            Modbus_Timer_Iint_Port(uint8 usart_reg)
*Description : 定时器初始化接口函数
*Arguments   : tick_kHz 定时器计数频率，单位为kHz
*Returns     : None
*Notes       : 注意该参数并不是指定时器时钟频率，而是指定时器计数的频率。
*******************************************************************************************************************
*/
boolean Modbus_Write_Hold2Bytes_Port(uint16 add,uint16 dat)
{
   holdbit[2*add] = (dat&0xff00)>>8;
   holdbit[2*add+1] = (dat&0xff);
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            Modbus_Timer_Iint_Port(uint8 usart_reg)
*Description : 定时器初始化接口函数
*Arguments   : tick_kHz 定时器计数频率，单位为kHz
*Returns     : None
*Notes       : 注意该参数并不是指定时器时钟频率，而是指定时器计数的频率。
*******************************************************************************************************************
*/
boolean Modbus_Read_Hold2Bytes_Port(uint16 add,uint16* buf)
{
   (*buf) = ((uint16)holdbit[add*2]<<8)|((uint16)holdbit[add*2+1]);
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            Modbus_Timer_Iint_Port(uint8 usart_reg)
*Description : 定时器初始化接口函数
*Arguments   : tick_kHz 定时器计数频率，单位为kHz
*Returns     : None
*Notes       : 注意该参数并不是指定时器时钟频率，而是指定时器计数的频率。
*******************************************************************************************************************
*/
void Modbus_User_FCList_Init_Port(modbus_st* modbus)
{
   
}





















