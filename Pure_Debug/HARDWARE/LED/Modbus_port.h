/*
******************************************************************************************************************
*Filename      :Modbus.h
*Programmer(s) :Nrush
*Description   :Modbus function
******************************************************************************************************************
*/
/* $ PAGE*/

#ifndef __MODBUS_PORT_H
#define __MODBUS_PORT_H

#include "usart.h"

#ifndef NULL
	#define NULL 0                   
#endif

#define MODBUS_RX_BUF_SIZE							((uint16)256)
#define MODBUS_TX_BUF_SIZE							((uint16)256)
#define MODBUS_TIMER_TICK							((uint32)1000)

#define MODBUS_TIMER_INIT(x)						Modbus_Timer_Init(x)
#define MODBUS_USART_INIT(x)						Modbus_Usart_Init(x)
#define MODBUS_SEND_BYTE(x)       {while((USART1->SR&0X40)==0);USART1->DR = x;} 
    

/*
******************************************************************************************************************
*                                           NO CHANGE !!
******************************************************************************************************************
*/
typedef  unsigned char      boolean;     /* Boolean value type. */
typedef  unsigned long int  uint32;      /* Unsigned 32 bit value */
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */
typedef  signed long int    int32;       /* Signed 32 bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
typedef  signed char        int8;        /* Signed 8  bit value */

enum{
   MODBUS_OFF = 0,
   MODBUS_ON  	
};

enum{
   MODBUS_NONE = 0,
   MODBUS_RECEIVE = 1,
   MODBUS_TIME_OUT = 2,
   MODBUS_RECEIVE_COMPLETE = 3      
};

enum{
   MODBUS_TIMER_NOTIMEOUT = 0,
   MODBUS_TIMER_BYTE_OUT,
   MODBUS_TIMER_FRAME_OUT,
   MODBUS_TIMER_RECEIVE_OUT
};

enum{
   MODBUS_TIMER_DISABLE = 0,
   MODBUS_TIMER_TX_EN,
   MODBUS_TIMER_RX_EN
};

typedef struct{
   uint32 tick_per_millisecond;
   uint32 modbus_timer_tick;
   uint8 timeout_flag;
   uint8 count_mode;
}modbus_timer_st;

#define MODBUS_USART_BAUDRATE_2400					((uint32)2400)
#define MODBUS_USART_BAUDRATE_4800					((uint32)4800)
#define MODBUS_USART_BAUDRATE_9600					((uint32)9600)
#define MODBUS_USART_BAUDRATE_14400					((uint32)14400)
#define MODBUS_USART_BAUDRATE_19200					((uint32)19200)
#define MODBUS_USART_BAUDRATE_28800					((uint32)28800)
#define MODBUS_USART_BAUDRATE_38400					((uint32)38400)
#define MODBUS_USART_BAUDRATE_57600					((uint32)57600)
#define MODBUS_USART_BAUDRATE_115200				((uint32)115200)

#define MODBUS_USART_WORDLENGTH_8B                  ((uint16)0x0000)

#define MODBUS_USART_STOPBITS_1                     ((uint16)0x0000)
#define MODBUS_USART_STOPBITS_2                     ((uint16)0x2000)

#define MODBUS_USART_PARITY_NO                      ((uint16)0x0000)
#define MODBUS_USART_PARITY_EVEN                    ((uint16)0x0400)
#define MODBUS_USART_PARITY_ODD                     ((uint16)0x0600)

#define MODBUS_USART_MODE                           ((uint16)0x000C)

#define MODBUS_USART_HARDWAREFLOWCONTROL_NONE       ((uint16)0x0000)
                                    

typedef struct{
  uint32 BaudRate;            
  uint16 WordLength;         
  uint16 StopBits;            
  uint16 Parity;             
  uint16 Mode;               
  uint16 HardwareFlowControl; 
}modbus_usart_st;

#define MODBUS_RTU									((uint8)0x0000)

#define MODBUS_MASTER								((uint8)0x0000)
#define MODBUS_SLAVER								((uint8)0x0001)

#define MODBUS_DEFAULT_ADDRESS						((uint8)0x01)

#define MODBUS_DEFAULT_RECEIVE_SPACE_TIME			((uint16)10)

typedef struct{
  uint8  modbus_mode;
  uint8  dev_mode;
  uint8  dev_address;
  uint32 one_byte_time;
  uint32 byte_space_time;
  uint32 frame_space_time; 
  uint16 receive_space_time;
}modbus_config_st;

enum{
   MODBUS_FREE = 0,
   MODBUS_TX,
   MODBUS_RX,
   MODBUS_ANALYSIS,
   MODBUS_COMPLETE
};

enum{
   MODBUS_NO_ERROR  = 0,
   MODBUS_BUF_FULL,
   MODBUS_RESPONSE_TIMEOUT,
   MODBUS_BYTE_TIME_OUT,
   MODBUS_ADD_ERR,
   MODBUS_RESPONSE_ABNORMAL,
   MODBUS_CRC_ERR,
   MODBUS_RESPONSE_FC_ERR,
   MODBUS_Master_NO_FC_ERR,
   MODBUS_MASTER_NO_FUNCTION_ERR,
   MODBUS_SLAVER_NO_FUNCTION_ERR
    
};

enum{
   MODBUS_RESPONSE_NO_ABNORMAL = 0x00,
   MODBUS_RESPONSE_ABNORMAL_1 = 0x01,
   MODBUS_RESPONSE_ABNORMAL_2 = 0x02,
   MODBUS_RESPONSE_ABNORMAL_3 = 0x03,
   MODBUS_RESPONSE_ABNORMAL_4 = 0x04,
   MODBUS_RESPONSE_ABNORMAL_5 = 0x05,
   MODBUS_RESPONSE_ABNORMAL_6 = 0x06,
   MODBUS_RESPONSE_ABNORMAL_8 = 0x08,
   MODBUS_RESPONSE_ABNORMAL_A = 0x0A,
   MODBUS_RESPONSE_ABNORMAL_B = 0x0B
};

typedef struct fc_list
{
	uint8	fc_id;
	void (*fc_m_rx)(void* ptr);
	void (*fc_s_rx)(void* ptr);
	struct fc_list* next;
}modbus_fc_list_st;

typedef struct{
	modbus_timer_st timer;
	modbus_usart_st usart;
	modbus_config_st config;
	uint16 usart_reg;
	uint16 rx_num;
	uint8 rx_irq;
	uint8 status;
	uint8 error_info;
	uint8 abnormal_info;
	uint8 rx_buf[MODBUS_RX_BUF_SIZE];
	uint8 tx_buf[MODBUS_TX_BUF_SIZE];
	modbus_fc_list_st list_head;
}modbus_st;


/*
******************************************************************************************************************
*                                          USER CONFIG
******************************************************************************************************************
*/











/*
******************************************************************************************************************
*                                            移植相关
******************************************************************************************************************
*/

/*
******************************************************************************************************************
*                                            移植无关
******************************************************************************************************************
*/
extern modbus_st modbus;
/*
******************************************************************************************************************
*                                            FUNCTION PROTOTYPES
******************************************************************************************************************
*/
void Modbus_List_Init(void);
extern void Modbus_Add_FC_List_Item(modbus_st* modbus,modbus_fc_list_st* add_item,uint8 fc_id,void (*fc_m_rx)(void* ptr),void (*fc_s_rx)(void* ptr));
extern uint8 Modbus_Remove_FC_List_Item(modbus_st* modbus,uint8 fc_id);
extern modbus_fc_list_st* Modbus_Find_FC_List_Item(modbus_st* modbus,uint8 fc_id);
extern void Clear_RX_Buf(modbus_st *modbus);
extern void Clear_TX_Buf(modbus_st *modbus,uint16 num);
extern void Modbus_Default_Parameter(modbus_st* modbus);
extern void Modbus_Receive_Data_IRQ(modbus_st* modbus,uint8 data);
extern void Modbus_Timer_Tick(modbus_st* modbus);
extern void Modbus_Receive_Data(modbus_st* modbus);
extern void Modbus_Load_Parameter(modbus_st* modbus);
extern void Modbus_Save_Parameter(modbus_st* modbus);
extern void Modbus_Init(modbus_st* modbus);
extern void Modbus_SendData(modbus_st* modbus,uint16  Outlength);
void Modbus_Timer_Init(modbus_timer_st timer);
void Modbus_Usart_Init(modbus_usart_st usart);
void Reset_Timer(modbus_st *modbus);


#endif
