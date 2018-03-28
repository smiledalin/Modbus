/*
******************************************************************************************************************
*Filename      :Modbus_RTU_Slaver_Port_V1.0.h
*Programmer(s) :Nrush
*Description   :Modbus库对外接口头文件
******************************************************************************************************************
*/
/* $ PAGE*/

/*
******************************************************************************************************************
*                                            MACRO
*Description : 配置宏定义
*Notes       : 该部分内容用于配置Modbus_RTU_Slaver的相关特性
******************************************************************************************************************
*/

#define MODBUS_DEFAULT_ADDRESS					((uint8)0x01)						   /* 默认地址*/
#define MODBUS_DEFAULT_BOUND					   MODBUS_USART_BAUDRATE_115200		/* 默认波特率*/
#define MODBUS_DEFAULT_PARITY					   MODBUS_USART_PARITY_NO				/* 默认校验方式*/
#define MODBUS_DEFAULT_USARTID					MODBUS_USART1						   /* 默认串口ID*/
		
#define MODBUS_RX_BUF_SIZE						   ((uint16)256)						   /* 接收缓冲区大小设置 */
#define MODBUS_TIMER_TICK						   ((uint16)1000)						   /* 定时器时钟频率kHz,至少25kHz，最高不大于1MHz*/
   
#define MODBUS_PARAMETER_BASE_ADDRESS			((uint32)0x00)						/* Modbus数据保存基地址*/
#define MODBUS_SAVE_KEYWORD						((uint8)0xA6)						/* Modbus数据储存记录密钥*/

#define MODBUS_HOLDBIT_REG_START				   (0)									/* Modbus位保持寄存器起始地址*/
#define MODBUS_HOLDBIT_REG_MAX_LENGTH			(32)                          /* Modbus位保持寄存器长度*/

#define MODBUS_INBIT_REG_START					(0)									/* Modbus位输入寄存器起始地址*/
#define MODBUS_INBIT_REG_MAX_LENGTH				(32)                          /* Modbus位输入寄存器长度*/

#define MODBUS_HOLDBYTE_REG_START				(0)									/* Modbus16位保持寄存器起始地址*/
#define MODBUS_HOLDBYTE_REG_MAX_LENGTH			(2)                           /* Modbus16位保持寄存器长度*/

#define MODBUS_INBYTE_REG_START					(0)									/* Modbus16位输入寄存器起始地址*/
#define MODBUS_INBYTE_REG_MAX_LENGTH			(2)                           /* Modbus16位输入寄存器长度*/

#define MODBUS_MAX_RW_BITS						(2000)								   /* Modbus位读写最大长度*/
#define MODBUS_MAX_RW_2BYTES					(128)                            /* Modbus寄存器读写最大长度*/

//#define MODBUS_DEBUG

/* Modbus功能码支持选择，置0则不包含该功能码，置1则启用功能码*/
#define MODBUS_FC01  (1)
#define MODBUS_FC02  (1)
#define MODBUS_FC03  (1)
#define MODBUS_FC04  (1)
#define MODBUS_FC05  (1)
#define MODBUS_FC06  (1)
#define MODBUS_FC15  (1)
#define MODBUS_FC16  (1)
/*
******************************************************************************************************************
*                                           STRUCT & MACRO
*Description : 数据结构及相关宏定义
*Notes       : 该部分内容请务必不要改动！
******************************************************************************************************************
*/

#ifndef NULL
	#define NULL 0  					 /* 定义空 */                
#endif

typedef  unsigned char      boolean;     /* Boolean value type. */
typedef  unsigned long int  uint32;      /* Unsigned 32 bit value */
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */
typedef  signed long int    int32;       /* Signed 32 bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
typedef  signed char        int8;        /* Signed 8  bit value */

#define	MODBUS_HIGH_BYTE(x)				(((x&0xFF00)>>8))			/*取16位高字节*/				
#define	MODBUS_LOW_BYTE(x)				(x&0xFF)					/*取16位低字节*/
#define	MODBUS_COMBINATION_BYTES(x,y)	((((uint16)x)<<8)|y)		/*将两字节组成16位字*/				

enum{									 /*  */  
	MODBUS_OFF = 0,
	MODBUS_ON  	
};

/*
*************************************定时器数据********************************************************************
*/

#define	MODBUS_BYTE_SPACE_RATIO			3			/*字节超时比率*/
#define	MODBUS_FRAME_SPACE_RATIO		7			/*帧超时比率*/

typedef struct{								/*定时器数据*/
	uint8 timeout_flag;						/*超时标志*/
	uint16 half_byte_time;					/*半字节时间单位为节拍数*/					
}modbus_timer_st;

/*
*************************************串口参数定义********************************************************************
*/

#define	MODBUS_READ_BOUND(x)						   (((uint8)x)&((uint8)0x3C))
#define	MODBUS_WRITE_BOUND(reg,data)				(reg = (((uint8)reg)&((uint8)0xC3))|data)	

#define MODBUS_USART_BAUDRATE_2400					((uint8)0x00)
#define MODBUS_USART_2400_HBT_US					   ((uint16)2292)
#define MODBUS_USART_BAUDRATE_4800					((uint8)0x04)
#define MODBUS_USART_4800_HBT_US					   ((uint16)1146)
#define MODBUS_USART_BAUDRATE_9600					((uint8)0x08)
#define MODBUS_USART_9600_HBT_US					   ((uint16)573)
#define MODBUS_USART_BAUDRATE_14400					((uint8)0x0C)
#define MODBUS_USART_14400_HBT_US					((uint16)382)
#define MODBUS_USART_BAUDRATE_19200					((uint8)0x10)
#define MODBUS_USART_19200_HBT_US					((uint16)286)
#define MODBUS_USART_BAUDRATE_28800					((uint8)0x14)
#define MODBUS_USART_28800_HBT_US					((uint16)191)
#define MODBUS_USART_BAUDRATE_38400					((uint8)0x18)
#define MODBUS_USART_38400_HBT_US					((uint16)143)
#define MODBUS_USART_BAUDRATE_57600					((uint8)0x1C)
#define MODBUS_USART_57600_HBT_US					((uint16)96)
#define MODBUS_USART_BAUDRATE_115200				((uint8)0x20)
#define MODBUS_USART_115200_HBT_US					((uint16)48)

#define	MODBUS_READ_PARITY(x)						(((uint8)x)&((uint8)0x03))
#define	MODBUS_WRITE_PARITY(reg,data)				(reg = (((uint8)reg)&((uint8)0xFC))|data)	

#define MODBUS_USART_PARITY_NO                   ((uint8)0x00)
#define MODBUS_USART_PARITY_EVEN                 ((uint8)0x01)
#define MODBUS_USART_PARITY_ODD                  ((uint8)0x02)

#define	MODBUS_READ_USARTID(x)						  (((uint8)x)&((uint8)0xC0))
#define	MODBUS_WRITE_USARTID(reg,data)			  (reg = (((uint8)reg)&((uint8)0x3F))|data)	

#define MODBUS_USART1                  			  ((uint8)0x00)
#define MODBUS_USART2								     ((uint8)0x40)
#define MODBUS_USART3								     ((uint8)0x80)
#define MODBUS_USART4								     ((uint8)0xC0)
					
/*状态标识*/
enum{												
	MODBUS_FREE = 0,								/*空闲状态*/
	MODBUS_RX,										/*接收状态*/
	MODBUS_ANALYSIS,								/*分析状态*/
	MODBUS_COMPLETE								/*完成状态*/
};

/*错误标识*/
enum{												
	MODBUS_NO_ERROR  = 0,
	MODBUS_RESPONSE_ABNORMAL_1 = 0x01,
	MODBUS_RESPONSE_ABNORMAL_2 = 0x02,
	MODBUS_RESPONSE_ABNORMAL_3 = 0x03,
	MODBUS_RESPONSE_ABNORMAL_4 = 0x04,
	MODBUS_RESPONSE_ABNORMAL_5 = 0x05,
	MODBUS_RESPONSE_ABNORMAL_6 = 0x06,
	MODBUS_RESPONSE_ABNORMAL_8 = 0x08,
	MODBUS_RESPONSE_ABNORMAL_A = 0x0A,
	MODBUS_RESPONSE_ABNORMAL_B = 0x0B,
	MODBUS_BUF_FULL_ERR,								   /*接收缓冲器满错误*/
	MODBUS_BYTE_TIMEOUT_ERR,							/*字节超时错误*/
	MODBUS_CRC_ERR,										/*CRC校验错误*/
	MODBUS_SLAVER_NO_FUNCTION_ERR,					/*从机功能码无处理函数错误*/ 
	MODBUS_ADD_ERR										   /*地址错误*/
};

/*函数链表*/
typedef struct fc_list								
{
	uint8	fc_id;									      /*功能码*/
	void (*fc_s_rx)(void* ptr);						/*从机功能码处理函数*/
	struct fc_list* next;
}modbus_fc_list_st;

/*Modbus数据结构*/
typedef struct{
	modbus_timer_st timer;							/*定时器数据*/
	uint8 usart_reg;								   /*串口数据*/
	uint16 rx_num;									   /*接收数据计数*/
	uint8 status;									   /*状态记录*/
	uint8 error_info;								   /*错误记录*/
	uint8 rx_buf[MODBUS_RX_BUF_SIZE];			/*接收缓冲区*/
	uint8 dev_address;								/*从机地址*/
	modbus_fc_list_st list_head;					/*功能函数链表*/
}modbus_st;

extern modbus_st modbus;							/*从机控制数据*/

extern void Modbus_Init(modbus_st* modbus);												/*Modbus初始化*/
extern void Modbus_List_Init(modbus_st* modbus);					               /*初始化Modbus功能码实现函数列表*/
extern void Modbus_Save_Parameter(modbus_st* modbus);									/*将Modbus参数储存到存储器中*/
extern void Modbus_Add_FC_List_Item(modbus_st* modbus,modbus_fc_list_st* add_item,uint8 fc_id,void (*fc_s_rx)(void* ptr));		/*添加功能码至Modbus_RTU*/
extern void Modbus_Receive_Data(modbus_st* modbus);									/*Modbus接收处理解码函数*/
extern void Modbus_Receive_Data_IRQ(modbus_st* modbus,uint8 data);				/*中断接收数据*/
extern void Modbus_Clear_RX_Buf(modbus_st *modbus);									/*清除Modbus接收缓存区并清除接收计数值*/
extern void Modbus_Reset_Slaver(modbus_st* modbus);                           /*重置Modbus从机状态*/
extern void Modbus_SendData(modbus_st* modbus,uint16  Outlength);             /*从机发送数据*/
extern void Modbus_Clear_TX_Buf(uint8 *buf,uint16 length);                    /*清除Modbus发送缓存*/
extern void Modbus_Refresh_Set(modbus_st* modbus);										/*Modbus更新设置参数*/
extern void Modbus_Save_Parameter(modbus_st* modbus);									/*将Modbus参数储存到存储器中*/
extern void Modbus_Load_Parameter(modbus_st* modbus);									/*从储存器中加载Modbus的所有参数*/
extern void Modbus_Default_Parameter(modbus_st* modbus);								/*重置Modbus的所有参数*/
extern void Modbus_Error_Slove(modbus_st* modbus);                                      /*Modbus从机错误状态处理*/
extern uint16 Modbus_Get_HBT_US(uint8 usart_reg);										          /*查询对应波特率半字节时间单位为us*/
extern boolean Modbus_Read_InBit_Data(uint16 base_add,uint16 length,uint8* buf);        /*读取输入位寄存器值*/
extern boolean Modbus_Read_HoldBit_Data(uint16 base_add,uint16 length,uint8* buf);      /*读取保持位寄存器值*/
extern boolean Modbus_Write_HoldBit_Data(uint16 base_add,uint16 length,uint8* buf);     /*写入保持位寄存器值*/
extern boolean Modbus_Read_In2Bytes_Data(uint16 base_add,uint16 length,uint8* buf);		 /*读取16位输入寄存器值*/
extern boolean Modbus_Read_Hold2Bytes_Data(uint16 base_add,uint16 length,uint8* buf);   /*读取16位保持寄存器值*/
extern boolean Modbus_Write_Hold2Bytes_Data(uint16 base_add,uint16 length,uint8* buf);  /*写入16位保持寄存器值*/

/*
*************************************用户移植时需要自行实现的函数（可根据需求裁剪）********************************************************************
*/

void Modbus_Usart_Init_Port(uint8 usart_reg);					   /*用来初始化串口的函数*/
void Modbus_Usart_IRQ_Disable_Port(uint8 usart_reg);			   /*用来失能串口中断的函数*/
void Modbus_Usart_IRQ_Enable_Port(uint8 usart_reg);				/*用来使能串口中断的函数*/
void Modbus_Send_Byte_Port(uint8 dat);							      /*串口发送一字节数据*/
void Modbus_Timer_Iint_Port(uint16 tick_kHz);					   /*用来初始化定时器的函数*/
void Modbus_Timer_IRQ_Time_Set_Port(uint16 ticks);				   /*用来设定定时器中断时间的函数*/
void Modbus_Timer_Clear_Port(void);								      /*用来重置并停止定时器及其中断的函数*/
void Modbus_Timer_Start_Port(void);								      /*用来启动定时器及其中断的函数*/
void Modbus_Save_Byte_Port(uint32 add,uint8 data);				   /*向储存器中保存一字节数据*/
void Modbus_User_FCList_Init_Port(modbus_st* modbus);	         /*用户自定义功能码实现*/
uint8 Modbus_Load_Byte_Port(uint32 add);						      /*从储存器中读取一字节数据*/
boolean Modbus_Read_InBit_Port(uint16 add,boolean* buf);		   /*读取输入线圈数据*/
boolean Modbus_Read_HoldBit_Port(uint16 add,boolean* buf);		/*读取保持线圈数据*/
boolean Modbus_Write_HoldBit_Port(uint16 add,boolean dat);		/*写入保持线圈数据*/
boolean Modbus_Read_In2Bytes_Port(uint16 add,uint16* buf);		/*读取16位输入寄存器数据*/
boolean Modbus_Read_Hold2Bytes_Port(uint16 add,uint16* buf);	/*读取16位保持寄存器数据*/
boolean Modbus_Write_Hold2Bytes_Port(uint16 add,uint16 dat);	/*写入16位保持寄存器数据*/



