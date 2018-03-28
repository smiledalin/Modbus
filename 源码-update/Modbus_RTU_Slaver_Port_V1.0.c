/*
******************************************************************************************************************
*Filename      :Modbus_RTU_Slaver_port_V1.0.c
*Programmer(s) :Nrush
*Description   :该文件为Modbus RTU从机通讯接口文件，与硬件平台相关
******************************************************************************************************************
*/
/* $ PAGE*/
#include "Modbus_RTU_Slaver_port_V1.0.h"		//对外接口，生成LIB文件后必须包含该文件
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
   uint8 temp; 
   
   /*假设我们的硬件平台初始化参数存放于USART_InitStructure结构体中，通过USART_Init(USART_InitStructure);初始化
   以下为推荐代码结构*/
   
   /*读取设置波特率进行波特率参数选择*/
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
		default:USART_InitStructure.USART_BaudRate = 9600;break;								/*其余默认为9600波特率*/
   }
   
   /*读取设置奇偶校验校验参数选择*/
   temp = MODBUS_READ_PARITY(usart_reg); 
    switch(temp)
   {
      case MODBUS_USART_PARITY_NO:  USART_InitStructure.USART_StopBits = USART_StopBits_2;				/*无校验时停止位为两位*/
                                    USART_InitStructure.USART_Parity = USART_Parity_No;
                                    USART_InitStructure.USART_WordLength = USART_WordLength_8b;break;
		case MODBUS_USART_PARITY_EVEN:USART_InitStructure.USART_StopBits = USART_StopBits_1;			/*偶校验时停止位为一位*/
                                    USART_InitStructure.USART_Parity = USART_Parity_Even;				
                                    USART_InitStructure.USART_WordLength = USART_WordLength_9b;break;	/*奇校验时停止位为一位*/
		case MODBUS_USART_PARITY_ODD:USART_InitStructure.USART_StopBits = USART_StopBits_1;				
                                    USART_InitStructure.USART_Parity = USART_Parity_Odd;
                                    USART_InitStructure.USART_WordLength = USART_WordLength_9b;break;
		default:USART_InitStructure.USART_StopBits = USART_StopBits_2;									/*其余默认为无校验*/
                                    USART_InitStructure.USART_Parity = USART_Parity_No;
                                    USART_InitStructure.USART_WordLength = USART_WordLength_8b;break;
   }
   
   /*读取串口号选择*/
   temp = MODBUS_READ_USARTID(usart_reg); 
    switch(temp)
   {
        case MODBUS_USART1:USART_InitStructure.USART = USART1;break;/*选择串口1进行通讯*/                            
		case MODBUS_USART2:USART_InitStructure.USART = USART2;break;/*选择串口2进行通讯*/ 		
		case MODBUS_USART3:USART_InitStructure.USART = USART3;break;/*选择串口3进行通讯*/ 				
		case MODBUS_USART4:USART_InitStructure.USART = USART4;break;/*选择串口4进行通讯*/ 					
		default:USART_InitStructure.USART = USART1;break;			/*其余默认选择串口1进行通讯*/ 
   }
   
   /*初始化串口*/
   USART_Init(USART_InitStructure);                    
}
/******************************************************************************************************************
*                                   void USART1_IRQHandler(void)
*Description : 串口中断函数（各硬件平台不同）
*Arguments   : None
*Returns     : None
*Notes       :	需要在此接收中断函数中调入Modbus_Receive_Data_IRQ(&modbus,Res)
				Res为接收到的字符。				
*******************************************************************************************************************
*/
void USART_IRQHandler(void)                	
{
	/*如果您准备使用多个串口请在对应的串口中断服务函数中添加如下代码，以下为
	串口1的推荐代码结构*/
	
	/*串口中断处理
	
		................................
		................................
		根据硬件平台编写
	
	*/
	
	/*MODBUS*/
	u8 Res;
    Res =USART_ReceiveData(USART1);			//读取接收到的数据
    Modbus_Receive_Data_IRQ(&modbus,Res);	//传递数据至Modbus内核
	
	/*串口中断处理
	
		................................
		................................
		根据硬件平台编写
	
	*/
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
  /*
  
  在此处添加清除串口中断标志和失能串口中断的代码。
  如串口号可选，可仿造Modbus_Usart_Init_Port()对modbus.usart_reg进行选择，下同不在重复说明。
  
  */
}
/******************************************************************************************************************
*                            Modbus_Usart_IRQ_Enable_Port(uint8 usart_reg)
*Description : 串口接收中断使能函数
*Arguments   : usart_reg 串口参数信息
*Returns     : None
*Notes       :	请保证使能前串口接收中断标志被清除。				
*******************************************************************************************************************
*/
void Modbus_Usart_IRQ_Enable_Port(uint8 usart_reg)
{	
  /*
  
  在此处添加清除串口中断标志和使能串口中断的代码。
  
  */
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
  /*
  
  在此处添加串口发送一字节数据的代码，代发送数据为dat。
  
  */
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
	/*
	
	如果您的硬件定时器为16位支持向上计数模式，
	则在此处添加配置定时器计数频率为tick_kHZ的向上计数的初始化代码。
	
	如果您的硬件定时器为8位或不支持向上计数模式，
	则在此处添配置定时器产生50us一次中断的初始化代码。
	
	*/
}
/******************************************************************************************************************
*                           TIM2_IRQHandler(void)
*Description : 串口中断服务函数（各硬件平台不同）
*Arguments   : None
*Returns     : None
*Notes       : 在该中断中需要对超时标志位累加
*******************************************************************************************************************
*/
void TIM_IRQHandler(void)
{	
	/*
	
	如果您的硬件定时器为16位支持向上计数模式，
	则在定时器中断中添加如下代码
	
	modbus.timer.timeout_flag++;
	
	如果您的硬件定时器为8位或不支持向上计数模式，
	则在定时器中断中添加如下代码
	
	modbus.timer.timeout_flag = modbus.timer.timeout_flag + 50;
	*/
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
	/*
	
	如果您的硬件定时器为16位支持向上计数模式，
	则在此添加设置定时器中断时间的代码，计数上溢出节拍为ticks。
	
	如果您的硬件定时器为8位或不支持向上计数模式，
	则在此添加如下代码，
	modbus.timer.half_byte_time = modbus.timer.half_byte_time / 50;
	*/
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
*Notes       : 若不开启MODBUS_STORE,则该部分不需要移植
*******************************************************************************************************************
*/
#if MODBUS_STORE == 1
void Modbus_Save_Byte_Port(uint32 add,uint8 data)		
{
	
}
#endif
/******************************************************************************************************************
*                            uint8 Modbus_Load_Byte_Port(uint32 add)
*Description : 从存储器读取一字节
*Arguments   : add 储存地址
*Returns     : 读得的数据值
*Notes       : 若不开启MODBUS_STORE,则该部分不需要移植
*******************************************************************************************************************
*/
#if MODBUS_STORE == 1
uint8 Modbus_Load_Byte_Port(uint32 add)				/*用来重置并开启定时器及其中断的函数*/
{	
	return 0;
}
#endif
/******************************************************************************************************************
*                           boolean Modbus_Read_InBit_Port(uint16 add,boolean* buf)
*Description : 读取输入线圈数据
*Arguments   : 待读取add地址，buf读出数据存放地址
*Returns     : MODBUS_OFF 读取成功 MODBUS_ON 读取失败
*Notes       : NONE
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
*                          boolean Modbus_Write_HoldBit_Port(uint16 add,boolean dat)
*Description : 写入保持线圈数据
*Arguments   : 待add写入地址，dat写入的数据
*Returns     : MODBUS_OFF 写入成功 MODBUS_ON 写入失败
*Notes       : NONE
*******************************************************************************************************************
*/
boolean Modbus_Write_HoldBit_Port(uint16 add,boolean dat)
{
   holdbit[add/8] = (~(0x01<<(add%8)))& holdbit[add/8];
   holdbit[add/8] = holdbit[add/8] | (dat<<(add%8));
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            boolean Modbus_Read_HoldBit_Port(uint16 add,boolean* buf)
*Description : 读取保持线圈数据
*Arguments   : 待读取add地址，buf读出数据存放地址
*Returns     : MODBUS_OFF 读取成功 MODBUS_ON 读取失败
*Notes       : NONE
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
*                            boolean Modbus_Read_In2Bytes_Port(uint16 add,uint16* buf)
*Description : 读取16位输入寄存器数据
*Arguments   : 待读取add地址，buf读出数据存放地址
*Returns     : MODBUS_OFF 读取成功 MODBUS_ON 读取失败
*Notes       : NONE
*******************************************************************************************************************
*/
boolean Modbus_Read_In2Bytes_Port(uint16 add,uint16* buf)
{
   (*buf) = ((uint16)holdbit[add*2]<<8)|((uint16)holdbit[add*2+1]);
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                          boolean Modbus_Write_Hold2Bytes_Port(uint16 add,uint16 dat)
*Description : 写入16位保持寄存器数据
*Arguments   : 待add写入地址，dat写入的数据
*Returns     : MODBUS_OFF 写入成功 MODBUS_ON 写入失败
*Notes       : NONE
*******************************************************************************************************************
*/
boolean Modbus_Write_Hold2Bytes_Port(uint16 add,uint16 dat)
{
   holdbit[2*add] = (dat&0xff00)>>8;
   holdbit[2*add+1] = (dat&0xff);
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            boolean Modbus_Read_Hold2Bytes_Port(uint16 add,uint16* buf)
*Description : 读取16位保持寄存器数据
*Arguments   : 待读取add地址，buf读出数据存放地址
*Returns     : MODBUS_OFF 读取成功 MODBUS_ON 读取失败
*Notes       : NONE
*******************************************************************************************************************
*/
boolean Modbus_Read_Hold2Bytes_Port(uint16 add,uint16* buf)
{
   (*buf) = ((uint16)holdbit[add*2]<<8)|((uint16)holdbit[add*2+1]);
   return MODBUS_OFF;
}
/******************************************************************************************************************
*                            Modbus_User_FCList_Init_Port(modbus_st* modbus)
*Description : 用户自定义功能码初始化函数
*Arguments   : modbus控制句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_User_FCList_Init_Port(modbus_st* modbus)
{
   
}





















