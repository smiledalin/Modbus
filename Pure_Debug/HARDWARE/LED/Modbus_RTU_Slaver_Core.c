/*
******************************************************************************************************************
*Filename      :Modbus_RTU_Slaver_Core.c
*Programmer(s) :Nrush
*Description   :该文件为Modbus RTU从机通讯核心代码与硬件平台无关，缺乏字节超时校验，其余功能完整，
******************************************************************************************************************
*/
/* $ PAGE*/
#include "Modbus_RTU_Slaver_port.h"		//对外接口，生成LIB文件后必须包含该文件
#include "stdio.h"
/*
******************************************************************************************************************
*                                           CONSTANTS & VARIABLE
*Description : 变量定义
*Notes       : none
******************************************************************************************************************
*/

modbus_st modbus;		//定义modbus结构体，包含Modbus所需的全部变量

/*---------------------------------------------------------------------------
CRC16 高位字节值表
---------------------------------------------------------------------------*/
static unsigned char const Modbus_CRC16Hi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
/*---------------------------------------------------------------------------
CRC16低位字节值表
---------------------------------------------------------------------------*/
static unsigned char const Modbus_CRC16Lo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/******************************************************************************************************************
*                                    unsigned int Modbus_CRC16(unsigned char *puchMsg, unsigned int usDataLen)
*Description : Modbus CRC16校验计算
*Arguments   : puchMsg：数据头指针 usDataLen：数据长度
*Returns     : CRC校验值（16位）
*Notes       : 写入时要将字节拆分为高低字节
*******************************************************************************************************************
*/
static unsigned int Modbus_CRC16(unsigned char *puchMsg, unsigned char usDataLen)
{
 unsigned char uchCRCHi = 0xFF ;        		// 高CRC字节初始化 
 unsigned char uchCRCLo = 0xFF ;       			// 低CRC 字节初始化
 unsigned char uIndex ;                      	// CRC循环中的索引 
 while (usDataLen--)                    		// 传输消息缓冲区 
 {
 uIndex = uchCRCHi ^ *puchMsg++ ;       		// 计算CRC
 uchCRCHi = uchCRCLo ^ Modbus_CRC16Hi[uIndex] ;
 uchCRCLo = Modbus_CRC16Lo[uIndex] ;
}
 return (uchCRCHi << 8 | uchCRCLo) ;
}
/******************************************************************************************************************
* 									void Modbus_Add_FC_List_Item()
*Description : 添加功能码至Modbus_RTU
*Arguments   : modbus Modbus控制器的句柄	add_item待添加功能码控制块 fc_id功能码代码
			   fc_s_rx功能码执行函数指针
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Add_FC_List_Item(modbus_st* modbus,modbus_fc_list_st* add_item,uint8 fc_id,void (*fc_s_rx)(void* ptr))
{
	modbus_fc_list_st* index_ptr;

	(*add_item).fc_id = fc_id;
	(*add_item).fc_s_rx = fc_s_rx;
	(*add_item).next = NULL;

	index_ptr = &((*modbus).list_head);

	while((*index_ptr).next != NULL)
	{
		index_ptr = (*index_ptr).next;
	}

	(*index_ptr).next = add_item;
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 移除功能码Modbus_RTU
*Arguments   : modbus Modbus控制器的句柄	fc_id待移除功能码代码
*Returns     : MODBUS_OFF 移除正常	MODBUS_ON	移除出错
*Notes       : None
*******************************************************************************************************************
*/
uint8 Modbus_Remove_FC_List_Item(modbus_st* modbus,uint8 fc_id)
{
	modbus_fc_list_st* index_ptr;
	modbus_fc_list_st* pre_ptr;

	index_ptr = (*modbus).list_head.next;
	pre_ptr = &((*modbus).list_head);

	while((index_ptr != NULL) && ((*index_ptr).fc_id != fc_id))
	{
		pre_ptr = index_ptr;
		index_ptr = (*index_ptr).next;
	}

	if(index_ptr != NULL)
	{
		(*pre_ptr).next = (*index_ptr).next;
		(*index_ptr).next = NULL;
		return MODBUS_OFF;
	}
	else
	{
		return MODBUS_ON;
	}
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 查询Modbus_RTU功能码控制块
*Arguments   : modbus Modbus控制器的句柄	fc_id待查询功能码代码
*Returns     : 查询Modbus_RTU功能码控制块地址
*Notes       : None
*******************************************************************************************************************
*/
modbus_fc_list_st* Modbus_Find_FC_List_Item(modbus_st* modbus,uint8 fc_id)
{
	modbus_fc_list_st* index_ptr;
	index_ptr = (*modbus).list_head.next;

	while((index_ptr != NULL) && (*index_ptr).fc_id != fc_id)
	{
		index_ptr = (*index_ptr).next;
	}

	return index_ptr;
}

/******************************************************************************************************************
*                                   uint16 Modbus_Get_HBT_US(uint8 usart_reg)
*Description : 查询对应波特率半字节时间单位为us
*Arguments   : usart_reg Modbus串口控制寄存器
*Returns     : 半字节时间单位为us
*Notes       : NONE
*******************************************************************************************************************
*/
uint16 Modbus_Get_HBT_US(uint8 usart_reg)
{
	uint16 temp = 0;
	
	switch(MODBUS_READ_BOUND(usart_reg))
	{
		case MODBUS_USART_BAUDRATE_2400:temp = MODBUS_USART_2400_HBT_US;break;
		case MODBUS_USART_BAUDRATE_4800:temp = MODBUS_USART_4800_HBT_US;break;
		case MODBUS_USART_BAUDRATE_9600:temp = MODBUS_USART_9600_HBT_US;break;
		case MODBUS_USART_BAUDRATE_14400:temp = MODBUS_USART_14400_HBT_US;break;
		case MODBUS_USART_BAUDRATE_19200:temp = MODBUS_USART_19200_HBT_US;break;
		case MODBUS_USART_BAUDRATE_28800:temp = MODBUS_USART_28800_HBT_US;break;
		case MODBUS_USART_BAUDRATE_38400:temp = MODBUS_USART_38400_HBT_US;break;
		case MODBUS_USART_BAUDRATE_57600:temp = MODBUS_USART_57600_HBT_US;break;
		case MODBUS_USART_BAUDRATE_115200:temp = MODBUS_USART_115200_HBT_US;break;
		default:temp = 0;break;
	}
	
	return temp;
}
/******************************************************************************************************************
*                                   void Clear_RX_Buf(modbus_st *modbus)
*Description : 清除Modbus接收缓存区并清除接收计数值
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Clear_RX_Buf(modbus_st *modbus)
{
	uint16 i=0;
	for(i = 0;i<(*modbus).rx_num;i++)
	{
		(*modbus).rx_buf[i] = 0;
	}
	(*modbus).rx_num = 0;
}
/******************************************************************************************************************
*                                   void Clear_RX_Buf(modbus_st *modbus)
*Description : 清除Modbus接收缓存区并清除接收计数值
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Clear_TX_Buf(uint8 *buf,uint16 length)
{
	uint16 i=0;
	for(i = 0;i<length;i++)
	{
		*(buf+i) = 0;
	}
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_SendData(modbus_st* modbus,uint16  Outlength)
{  
    uint8 i;
	uint16 temp;
	temp = Modbus_CRC16((*modbus).rx_buf,Outlength);
	(*modbus).rx_buf[Outlength] = temp&0xFF;
	(*modbus).rx_buf[Outlength+1] = (temp&0xFF00) >> 8;
	(*modbus).rx_buf[0] = (*modbus).dev_address;

	for(i = 0;i<(Outlength + 2);i++)
	{
		Modbus_Send_Byte_Port((*modbus).rx_buf[i]);
	}
	
	Clear_TX_Buf((*modbus).rx_buf,Outlength + 2);
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Reset_Slaver(modbus_st* modbus)
{
	Modbus_Timer_Clear_Port();
	(*modbus).status = MODBUS_FREE;
	(*modbus).timer.timeout_flag = 0;
	Clear_RX_Buf(modbus);
	(*modbus).error_info = MODBUS_NO_ERROR;
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Error_Slove(modbus_st* modbus)
{
	uint8 temp_buf = 0;
	uint8 num_counter = 0;
	if((*modbus).error_info == MODBUS_BYTE_TIMEOUT_ERR)
	{
		Modbus_Reset_Slaver(modbus);
		printf("MODBUS_BYTE_TIMEOUT_ERR\r\n");
	}
	else if((*modbus).error_info == MODBUS_BUF_FULL_ERR)
	{	
		Modbus_Reset_Slaver(modbus);	
		printf("MODBUS_BUF_FULL_ERR\r\n");		
	}
	else if((*modbus).error_info == MODBUS_CRC_ERR)
	{
		Modbus_Reset_Slaver(modbus);
		printf("MODBUS_CRC_ERR\r\n");
	}
	else if((*modbus).error_info == MODBUS_SLAVER_NO_FUNCTION_ERR)
	{
		Modbus_Reset_Slaver(modbus);	
		printf("MODBUS_SLAVER_NO_FUNCTION_ERR\r\n");
	}
   else if((*modbus).error_info == MODBUS_ADD_ERR)
	{
		Modbus_Reset_Slaver(modbus);	
		printf("ADDR_ERR\r\n");
	}		
	else if((*modbus).error_info == MODBUS_RESPONSE_ABNORMAL_1)
	{
		num_counter = 1;
		temp_buf = (*modbus).rx_buf[1];
		Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
		Modbus_Reset_Slaver(modbus);
		(*modbus).rx_buf[num_counter++] = temp_buf|0x80;
		(*modbus).rx_buf[num_counter++] = MODBUS_RESPONSE_ABNORMAL_1;
		Modbus_SendData(modbus,num_counter);		
		printf("MODBUS_RESPONSE_ABNORMAL_1\r\n");
		Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);
	}
	else if((*modbus).error_info == MODBUS_RESPONSE_ABNORMAL_2)
	{
		num_counter = 1;
		temp_buf = (*modbus).rx_buf[1];
		Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
		Modbus_Reset_Slaver(modbus);
		(*modbus).rx_buf[num_counter++] = temp_buf|0x80;
		(*modbus).rx_buf[num_counter++] = MODBUS_RESPONSE_ABNORMAL_2;
		Modbus_SendData(modbus,num_counter);		
		printf("MODBUS_RESPONSE_ABNORMAL_2\r\n");
      Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);
	}
	else if((*modbus).error_info == MODBUS_RESPONSE_ABNORMAL_3)
	{
		num_counter = 1;
		temp_buf = (*modbus).rx_buf[1];
		Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
		Modbus_Reset_Slaver(modbus);
		(*modbus).rx_buf[num_counter++] = temp_buf|0x80;
		(*modbus).rx_buf[num_counter++] = MODBUS_RESPONSE_ABNORMAL_3;
		Modbus_SendData(modbus,num_counter);		
		printf("MODBUS_RESPONSE_ABNORMAL_3\r\n");
		Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);
	}
	else if((*modbus).error_info == MODBUS_RESPONSE_ABNORMAL_4)
	{
		num_counter = 1;
		temp_buf = (*modbus).rx_buf[1];
		Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
		Modbus_Reset_Slaver(modbus);
		(*modbus).rx_buf[num_counter++] = temp_buf|0x80;
		(*modbus).rx_buf[num_counter++] = MODBUS_RESPONSE_ABNORMAL_4;
		Modbus_SendData(modbus,num_counter);		
		printf("MODBUS_RESPONSE_ABNORMAL_4\r\n");
		Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);
	}
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : 该函数用来重置Modbus所有参数。默认参数设置如下：
				波特率：9600	奇偶校验：无校验
*******************************************************************************************************************
*/
void Modbus_Default_Parameter(modbus_st* modbus)
{	
	MODBUS_WRITE_BOUND((*modbus).usart_reg,MODBUS_DEFAULT_BOUND);
	MODBUS_WRITE_PARITY((*modbus).usart_reg,MODBUS_DEFAULT_PARITY);
	MODBUS_WRITE_USARTID((*modbus).usart_reg,MODBUS_DEFAULT_USARTID);
	(*modbus).dev_address = MODBUS_DEFAULT_ADDRESS;
}

/******************************************************************************************************************
*                                   void Modbus_Load_Parameter(modbus_st* modbus)
*Description : 从储存器中加载Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : 加载参数后并不会生效，只有调用Modbus_Rresh_Set()后才会生效
*******************************************************************************************************************
*/
void Modbus_Load_Parameter(modbus_st* modbus)
{	
	if(MODBUS_SAVE_KEYWORD == Modbus_Load_Byte_Port(MODBUS_PARAMETER_BASE_ADDRESS+2))
	{
		(*modbus).usart_reg = Modbus_Load_Byte_Port(MODBUS_PARAMETER_BASE_ADDRESS);
		(*modbus).dev_address = Modbus_Load_Byte_Port(MODBUS_PARAMETER_BASE_ADDRESS+1);					
	}
	else
	{
		Modbus_Default_Parameter(modbus);
		Modbus_Save_Parameter(modbus);
	}

}
/******************************************************************************************************************
*                                   void Modbus_Save_Parameter(modbus_st* modbus)
*Description : 将Modbus参数储存到存储器中
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Save_Parameter(modbus_st* modbus)
{
	Modbus_Save_Byte_Port(MODBUS_PARAMETER_BASE_ADDRESS,(*modbus).usart_reg);
	Modbus_Save_Byte_Port(MODBUS_PARAMETER_BASE_ADDRESS+1,(*modbus).dev_address);
	Modbus_Save_Byte_Port(MODBUS_PARAMETER_BASE_ADDRESS+2,MODBUS_SAVE_KEYWORD);
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : 该函数用来重置Modbus所有参数。默认参数设置如下：
				波特率：9600	奇偶校验：无校验
*******************************************************************************************************************
*/
void Modbus_Rresh_Set(modbus_st* modbus)
{
	uint16 temp = 0;
	temp = Modbus_Get_HBT_US((*modbus).usart_reg);
	
	(*modbus).timer.timeout_flag			= 0;
	(*modbus).timer.half_byte_time			= temp/(1000/MODBUS_TIMER_TICK);		
	
	(*modbus).status = MODBUS_FREE;
	(*modbus).error_info = MODBUS_NO_ERROR;
	(*modbus).rx_num = MODBUS_RX_BUF_SIZE;
	Clear_RX_Buf(modbus);
		
	Modbus_Usart_Init_Port((*modbus).usart_reg);
	printf("USART Init OK\r\n");
	Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);
	Modbus_Timer_Iint_Port(MODBUS_TIMER_TICK);
	Modbus_Timer_IRQ_Time_Set_Port((*modbus).timer.half_byte_time);
	Modbus_Timer_Clear_Port();
}

/******************************************************************************************************************
*                                   void Modbus_Init(modbus_st* modbus)
*Description : Modbus初始化
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Init(modbus_st* modbus)
{
	Modbus_Load_Parameter(modbus);
	Modbus_Rresh_Set(modbus);
	
	(*modbus).list_head.fc_id = 0x00;
	(*modbus).list_head.fc_s_rx = NULL;
	(*modbus).list_head.next = NULL;
	
	Modbus_List_Init_Port(modbus);
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Receive_Data_IRQ(modbus_st* modbus,uint8 data)
{
	Modbus_Timer_Clear_Port();
   (*modbus).timer.timeout_flag = 0;
	
	if((*modbus).status == MODBUS_FREE)
	{
		(*modbus).status = MODBUS_RX;
	}
	
	if((*modbus).timer.timeout_flag <= MODBUS_BYTE_SPACE_RATIO)
	{
		if((*modbus).status == MODBUS_RX)
		{
			
			if((*modbus).rx_num >= MODBUS_RX_BUF_SIZE)
			{
				(*modbus).error_info = MODBUS_BUF_FULL_ERR;
			}
			else
			{
				(*modbus).rx_buf[(*modbus).rx_num] = data;
				(*modbus).rx_num++;
			}
		}
	}
	else
	{
		(*modbus).error_info = MODBUS_BYTE_TIMEOUT_ERR;
	}

	Modbus_Timer_Start_Port();	
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄st* 
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static void BE_Check(modbus_st *modbus)	/*帧头尾格式检测*/
{
	unsigned int temp = 0;
	temp = Modbus_CRC16((*modbus).rx_buf,(*modbus).rx_num-2);

	if((*modbus).rx_buf[0] == (*modbus).dev_address)	/*地址校验*/
	{
		if(((temp&0xFF) == (*modbus).rx_buf[(*modbus).rx_num - 2]) && (((temp&0xFF00)>>8) == (*modbus).rx_buf[(*modbus).rx_num - 1]))	/*CRC校验*/
		{
			(*modbus).error_info = MODBUS_NO_ERROR;
		}
		else
		{
			(*modbus).error_info = MODBUS_CRC_ERR;
		}
	}
	else
	{

      (*modbus).error_info = MODBUS_ADD_ERR;
	}
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static void FC_Realized(modbus_st *modbus)	/*功能码功能实现*/
{
	modbus_fc_list_st* index_ptr = NULL;
	index_ptr = Modbus_Find_FC_List_Item(modbus,(*modbus).rx_buf[1]);
	
	if(index_ptr != NULL)  /*检测功能码是否异常*/
	{
		if((*index_ptr).fc_s_rx != NULL)
		{
			(*(*index_ptr).fc_s_rx)((void*)modbus);			
		}
		else
		{
			/*功能码执行函数未定义*/
			(*modbus).error_info = MODBUS_SLAVER_NO_FUNCTION_ERR;
		}
	}
	else
	{
		 /*非法功能码否异常*/
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_1;
	}
	
	if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Receive_Data(modbus_st* modbus)
{
	/*Modbus从机模式下接收程序*/
	if((*modbus).status == MODBUS_RX)	/*第二阶段测试接收是否完成或是否发生超时错误*/
		{			
			while(((*modbus).timer.timeout_flag < MODBUS_FRAME_SPACE_RATIO)&& ((*modbus).error_info == MODBUS_NO_ERROR));//这里为了保证实时性使用循环查询
			
			if((*modbus).error_info != MODBUS_NO_ERROR)
				{
					Modbus_Error_Slove(modbus);					
				}
			else
				{
					(*modbus).status = MODBUS_ANALYSIS;
					Modbus_Timer_Clear_Port();
				}
		}
	else if((*modbus).status == MODBUS_ANALYSIS)	/*第三阶段分析数据格式是否正确并调用解析函数进行相应解析*/
		{
			BE_Check(modbus);						/*对帧首尾进行校验*/
			if((*modbus).error_info == MODBUS_NO_ERROR)
				{
					FC_Realized(modbus);				/*功能码校验实现*/
				}
			else
				{
					Modbus_Error_Slove(modbus);	
				}
		}
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
boolean Modbus_Read_InBit_Data(uint16 base_add,uint16 length,uint8* buf)
{
	uint16 i;
	uint8 j;
	uint8 temp;
	
	(*buf) = 0;
	
	for(i=0;i<length;i++)
	{
		if(Modbus_Read_InBit_Port(base_add + i,&temp) == MODBUS_OFF)
		{
			j = i%8;			
			if((j == 0) && (i > 0))
			{
				buf++;
				(*buf) = 0;
			}
			temp = temp<<j;
			(*buf) = (*buf)|temp;
		}
		else
		{
			return MODBUS_ON;
		}					
	}
	
	return MODBUS_OFF;
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
boolean Modbus_Read_HoldBit_Data(uint16 base_add,uint16 length,uint8* buf)
{
	uint16 i;
	uint8 j;
	uint8 temp;
	
	(*buf) = 0;
	
	for(i=0;i<length;i++)
	{
		if(Modbus_Read_HoldBit_Port(base_add + i,&temp) == MODBUS_OFF)
		{
			j = i%8;			
			if((j == 0) && (i > 0))
			{
				buf++;
				(*buf) = 0;
			}
			temp = temp<<j;
			(*buf) = (*buf)|temp;
		}
		else
		{
			return MODBUS_ON;
		}					
	}
	
	return MODBUS_OFF;
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
boolean Modbus_Write_HoldBit_Data(uint16 base_add,uint16 length,uint8* buf)
{
	uint16 i;
	uint8 j;
	boolean temp;
	
	for(i=0;i<length;i++)
	{
		j = i%8;
		if((*buf)&(0x01<<j))
		{
			temp = 1;
		}
		else
		{ 
			temp = 0;
		}
		
		if(Modbus_Write_HoldBit_Port(base_add+i,temp) == MODBUS_OFF)
		{
			if(j == 0 && i>0)
			{
				buf++;
			}	
		}
		else
		{
			return MODBUS_ON;			
		}		
	}
	
	return MODBUS_OFF;
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
boolean Modbus_Read_In2Bytes_Data(uint16 base_add,uint16 length,uint8* buf)
{
	uint16 i;
	uint16 temp;
	
	
	for(i=0;i<length;i++)
	{
		if(Modbus_Read_In2Bytes_Port(base_add+i,&temp) == MODBUS_OFF)
		{
			(*buf) = (temp&0xFF00)>>8;
			buf++;	
			(*buf) = temp&0xFF;
			buf++;
		}
		else
		{
			return MODBUS_ON;
		}
	}
	
	return MODBUS_OFF;
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
boolean Modbus_Read_Hold2Bytes_Data(uint16 base_add,uint16 length,uint8* buf)
{
	uint16 i;
	uint16 temp;
	
	
	for(i=0;i<length;i++)
	{
		if(Modbus_Read_Hold2Bytes_Port(base_add+i,&temp) == MODBUS_OFF)
		{
			(*buf) = (temp&0xFF00)>>8;
			buf++;	
			(*buf) = temp&0xFF;
			buf++;
		}
		else
		{
			return MODBUS_ON;
		}
	}
	
	return MODBUS_OFF;
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : 重置Modbus的所有参数
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
boolean Modbus_Write_Hold2Bytes_Data(uint16 base_add,uint16 length,uint8* buf)
{
	uint16 i;
	uint16 temp;
	
	
	for(i=0;i<length;i++)
	{
		temp = ((uint16)(*buf)<<8) | (*(buf+1));	
		if(Modbus_Write_Hold2Bytes_Port(base_add+i,temp) == MODBUS_OFF)
		{
			buf = buf + 2;	
		}
		else
		{
			return MODBUS_ON;			
		}		
	}
	
	return MODBUS_OFF;
}


