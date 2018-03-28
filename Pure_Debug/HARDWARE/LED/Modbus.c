/*
******************************************************************************************************************
*Filename      :Modbus.c
*Programmer(s) :Nrush
*Description   :Modbus function
******************************************************************************************************************
*/
/* $ PAGE*/
#include "stdlib.h"
#include "Modbus_port.h"
#include "sys.h"

/*
******************************************************************************************************************
*                                            CONSTANTS & MACROS
*Description : ��������
*Notes       : none
******************************************************************************************************************
*/
modbus_st modbus;		//����modbus�ṹ�壬����Modbus�����ȫ������

/*---------------------------------------------------------------------------
CRC16 ��λ�ֽ�ֵ��
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
CRC16��λ�ֽ�ֵ��
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
*Description : CRCУ�����
*Arguments   : puchMsg������ͷָ�� usDataLen�����ݳ���
*Returns     : CRCУ��ֵ��16λ��
*Notes       : д��ʱҪ���ֽڲ��Ϊ�ߵ��ֽ�
*******************************************************************************************************************
*/
static unsigned int Modbus_CRC16(unsigned char *puchMsg, unsigned char usDataLen)
{
 unsigned char uchCRCHi = 0xFF ;        // ��CRC�ֽڳ�ʼ�� 
 unsigned char uchCRCLo = 0xFF ;        // ��CRC �ֽڳ�ʼ��
 unsigned char uIndex ;                      // CRCѭ���е����� 
 while (usDataLen--)                    // ������Ϣ������ 
 {
 uIndex = uchCRCHi ^ *puchMsg++ ;       // ����CRC
 uchCRCHi = uchCRCLo ^ Modbus_CRC16Hi[uIndex] ;
 uchCRCLo = Modbus_CRC16Lo[uIndex] ;
}
 return (uchCRCHi << 8 | uchCRCLo) ;
}

/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Default_Parameter(modbus_st* modbus)
{
	(*modbus).timer.tick_per_millisecond	= MODBUS_TIMER_TICK;
	(*modbus).timer.modbus_timer_tick		= 0;
	(*modbus).timer.timeout_flag			= MODBUS_TIMER_NOTIMEOUT;
	(*modbus).timer.count_mode				= MODBUS_TIMER_DISABLE;

	(*modbus).usart.BaudRate				= MODBUS_USART_BAUDRATE_115200;
	(*modbus).usart.WordLength				= MODBUS_USART_WORDLENGTH_8B;
	(*modbus).usart.StopBits				= MODBUS_USART_STOPBITS_2;
	(*modbus).usart.Parity					= MODBUS_USART_PARITY_NO;
	(*modbus).usart.Mode					= MODBUS_USART_MODE;
	(*modbus).usart.HardwareFlowControl		= MODBUS_USART_HARDWAREFLOWCONTROL_NONE;

	(*modbus).config.modbus_mode			= MODBUS_RTU;
	(*modbus).config.dev_mode				= MODBUS_SLAVER;
	(*modbus).config.dev_address			= 0x01;
	(*modbus).config.one_byte_time			= (uint32)((((1000.0f)/((float)(*modbus).usart.BaudRate))*(11.0f))*(*modbus).timer.tick_per_millisecond);
	(*modbus).config.byte_space_time		= (uint32)((2.0f)*(*modbus).config.one_byte_time);
	(*modbus).config.frame_space_time		= (uint32)((3.5f)*(*modbus).config.one_byte_time);
	(*modbus).config.receive_space_time		= (uint16)(10*(*modbus).timer.tick_per_millisecond);

	(*modbus).rx_num = 0;
	(*modbus).rx_irq = MODBUS_OFF;
	(*modbus).status = MODBUS_FREE;
	(*modbus).error_info = MODBUS_NO_ERROR;
	(*modbus).abnormal_info = MODBUS_RESPONSE_NO_ABNORMAL;
	(*modbus).rx_num = MODBUS_RX_BUF_SIZE;
	Clear_RX_Buf(modbus);
	Clear_TX_Buf(modbus,MODBUS_TX_BUF_SIZE);

	(*modbus).list_head.fc_id = 0x00;
	(*modbus).list_head.fc_m_rx = NULL;
	(*modbus).list_head.fc_s_rx = NULL;
	(*modbus).list_head.next = NULL;
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Add_FC_List_Item(modbus_st* modbus,modbus_fc_list_st* add_item,uint8 fc_id,void (*fc_m_rx)(void* ptr),void (*fc_s_rx)(void* ptr))
{
	modbus_fc_list_st* index_ptr;

	(*add_item).fc_id = fc_id;
	(*add_item).fc_m_rx = fc_m_rx;
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
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
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
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
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
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Load_Parameter(modbus_st* modbus)
{
	Modbus_Default_Parameter(modbus);
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Save_Parameter(modbus_st* modbus)
{

}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Init(modbus_st* modbus)
{
	Modbus_Default_Parameter(modbus);
	Modbus_Load_Parameter(modbus);		//�Ӵ������м���Modbus��ز���
 Modbus_Usart_Init((*modbus).usart);	//��ʼ��Modbus����
	Modbus_Timer_Init((*modbus).timer);	//��ʼ��Modbusר�ö�ʱ��	
	Modbus_List_Init();
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
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
	(*modbus).rx_irq = MODBUS_OFF;
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Clear_TX_Buf(modbus_st *modbus,uint16 num)
{
	uint16 i=0;
	for(i = 0;i<num;i++)
	{
		(*modbus).tx_buf[i] = 0;
	}
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Reset_Timer(modbus_st *modbus)
{
 TIM_Cmd(TIM2, DISABLE);	
 TIM_SetCounter(TIM2,0);
 TIM_ClearITPendingBit(TIM2,TIM_IT_CC1); 
 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  if(TIM_GetCounter(TIM2) != 0)
 {
   printf("err\r\n");
 }
	(*modbus).timer.count_mode = MODBUS_TIMER_DISABLE;
	(*modbus).timer.modbus_timer_tick = 0;
	(*modbus).timer.timeout_flag = MODBUS_TIMER_NOTIMEOUT;
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static void BE_Check(modbus_st *modbus)	/*֡ͷβ��ʽ���*/
{
	unsigned int temp = 0;
	temp = Modbus_CRC16((*modbus).rx_buf,(*modbus).rx_num-2);

 
	if((((*modbus).rx_buf[0] == (*modbus).tx_buf[0])&&((*modbus).config.dev_mode == MODBUS_MASTER))\
  || (((*modbus).rx_buf[0] == (*modbus).config.dev_address)&&((*modbus).config.dev_mode == MODBUS_SLAVER)))	/*��ַУ��*/
	{
		if(((temp&0xFF) == (*modbus).rx_buf[(*modbus).rx_num - 2]) && (((temp&0xFF00)>>8) == (*modbus).rx_buf[(*modbus).rx_num - 1]))	/*CRCУ��*/
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
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static void FC_Realized(modbus_st *modbus)	/*�����빦��ʵ��*/
{
	modbus_fc_list_st* index_ptr = NULL;

	if((*modbus).config.dev_mode == MODBUS_MASTER)
	{
		if((*modbus).rx_buf[1] == (*modbus).tx_buf[1])	/*��⹦���뷵���Ƿ��쳣*/
		{
			/*�����뷵������*/
			
			index_ptr = Modbus_Find_FC_List_Item(modbus,(*modbus).rx_buf[1]);

			if(index_ptr != NULL)
			{
				if((*index_ptr).fc_m_rx != NULL)
				{
					(*(*index_ptr).fc_m_rx)((void*)modbus);
				}
				else
				{
					(*modbus).error_info = MODBUS_MASTER_NO_FUNCTION_ERR;
				}
			}
			else
			{
				(*modbus).error_info = MODBUS_Master_NO_FC_ERR;
			}
		}
		else if((*modbus).rx_buf[1] == ((*modbus).tx_buf[1]|0x80))	
		{
			/*�����뷵���쳣*/
			(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL;
			(*modbus).abnormal_info = (*modbus).rx_buf[2];
		}
		else
		{
			/*�����뷵�ش���*/
			(*modbus).error_info = MODBUS_RESPONSE_FC_ERR;
		}
	}
	else if((*modbus).config.dev_mode == MODBUS_SLAVER)
	{
		index_ptr = Modbus_Find_FC_List_Item(modbus,(*modbus).rx_buf[1]);
		if(index_ptr != NULL)  /*��⹦�����Ƿ��쳣*/
		{
			if((*index_ptr).fc_s_rx != NULL)
			{
				(*(*index_ptr).fc_s_rx)((void*)modbus);			
			}
			else
			{
				/*������ִ�к���δ����*/
				(*modbus).error_info = MODBUS_SLAVER_NO_FUNCTION_ERR;
			}
		}
		else
		{
			 /*�Ƿ���������쳣*/
			(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL;
			(*modbus).abnormal_info = MODBUS_RESPONSE_ABNORMAL_1;
		}

	}
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Timer_Tick(modbus_st* modbus)
{
	if((*modbus).timer.count_mode != MODBUS_TIMER_DISABLE)
	{
		(*modbus).timer.modbus_timer_tick++;

		if((*modbus).timer.modbus_timer_tick > (*modbus).config.byte_space_time)
		{
			if((*modbus).timer.modbus_timer_tick < (*modbus).config.frame_space_time)
			{
				(*modbus).timer.timeout_flag = MODBUS_TIMER_BYTE_OUT;
			}
			else if((*modbus).timer.modbus_timer_tick < (*modbus).config.receive_space_time)
			{
				(*modbus).timer.timeout_flag = MODBUS_TIMER_FRAME_OUT;
			}
			else
			{
				(*modbus).timer.timeout_flag = MODBUS_TIMER_RECEIVE_OUT;
			}
		}

		if((*modbus).timer.count_mode == MODBUS_TIMER_TX_EN)
		{
			if((*modbus).timer.timeout_flag == MODBUS_TIMER_RECEIVE_OUT)
			{
				(*modbus).error_info = MODBUS_RESPONSE_TIMEOUT;
			}
		}
	}

	if((*modbus).rx_irq == MODBUS_ON)
	{
		(*modbus).timer.modbus_timer_tick = 0;
		(*modbus).rx_irq = MODBUS_OFF;
	}
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Receive_Data_IRQ(modbus_st* modbus,uint8 data)
{
 TIM_Cmd(TIM2, DISABLE);	
 TIM_SetCounter(TIM2,0);
 TIM_ClearITPendingBit(TIM2,TIM_IT_CC1); 
 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
 
	if((*modbus).config.dev_mode == MODBUS_SLAVER)
	{
		if((*modbus).status == MODBUS_FREE)
		{
			(*modbus).status = MODBUS_RX;
			(*modbus).timer.count_mode = MODBUS_TIMER_RX_EN;
		}
	}
	else if((*modbus).config.dev_mode == MODBUS_MASTER)
	{
		if((*modbus).status == MODBUS_TX)
		{
			(*modbus).status = MODBUS_RX;
			(*modbus).timer.count_mode = MODBUS_TIMER_RX_EN;
		}
	}

	if((*modbus).status == MODBUS_RX)
	{
		if((*modbus).rx_num < (MODBUS_RX_BUF_SIZE-1))
		{
			(*modbus).rx_buf[(*modbus).rx_num] = data;
			(*modbus).rx_num++;
		}
		else
		{
			(*modbus).error_info = MODBUS_BUF_FULL;
		}
	}

//	if((*modbus).timer.timeout_flag == MODBUS_TIMER_BYTE_OUT)
//	{
//		(*modbus).error_info = MODBUS_BYTE_TIME_OUT;
//	}

//	(*modbus).rx_irq = MODBUS_ON;

 TIM_Cmd(TIM2, ENABLE);	
}

void Modbus_Error_Slove(modbus_st* modbus)
{
 (*modbus).error_info = MODBUS_NO_ERROR;
}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_Receive_Data(modbus_st* modbus)
{
	if((*modbus).config.modbus_mode == MODBUS_RTU)
	{
		if((*modbus).config.dev_mode == MODBUS_MASTER)
		{
			/*Modbus����ģʽ�½��ճ���*/
			if((*modbus).status != MODBUS_FREE)
			{
				if((*modbus).status == MODBUS_TX)	/*��һ�׶β��Է���ʱ���Ƿ�ʱ*/
				{
					while(((*modbus).status == MODBUS_TX) && ((*modbus).error_info == MODBUS_NO_ERROR)); //����Ϊ�˱�֤ʵʱ��ʹ��ѭ����ѯ
					if((*modbus).error_info == MODBUS_RESPONSE_TIMEOUT)
					{
						/*��Ӧ��ʱ�����־*/
						(*modbus).status = MODBUS_FREE;
						Reset_Timer(modbus);	
					}
				}
				else if((*modbus).status == MODBUS_RX)	/*�ڶ��׶β��Խ����Ƿ���ɻ��Ƿ�����ʱ����*/
				{
					while((((*modbus).timer.timeout_flag == MODBUS_TIMER_NOTIMEOUT)||((*modbus).timer.timeout_flag == MODBUS_TIMER_BYTE_OUT))\
						&& ((*modbus).error_info == MODBUS_NO_ERROR)); //����Ϊ�˱�֤ʵʱ��ʹ��ѭ����ѯ
					
					if(((*modbus).error_info == MODBUS_BUF_FULL) || ((*modbus).error_info == MODBUS_BYTE_TIME_OUT))
					{
						(*modbus).status = MODBUS_FREE;
						Reset_Timer(modbus);
     
						Clear_RX_Buf(modbus);
					}
					else
					{
						(*modbus).status = MODBUS_ANALYSIS;
						Reset_Timer(modbus);
					}
				}
				else if((*modbus).status == MODBUS_ANALYSIS)	/*�����׶η������ݸ�ʽ�Ƿ���ȷ�����ý�������������Ӧ����*/
				{
						BE_Check(modbus);						/*��֡��β����У��*/
						if((*modbus).error_info == MODBUS_NO_ERROR)
						{
							FC_Realized(modbus);				/*������У��ʵ��*/
						}
						else
						{
							(*modbus).status = MODBUS_FREE;
       (*modbus).error_info = MODBUS_NO_ERROR;
							Clear_RX_Buf(modbus);
						}
				}
			}
		}
		else if((*modbus).config.dev_mode == MODBUS_SLAVER)
		{
			/*Modbus�ӻ�ģʽ�½��ճ���*/
			if((*modbus).status == MODBUS_RX)	/*�ڶ��׶β��Խ����Ƿ���ɻ��Ƿ�����ʱ����*/
				{
					while((((*modbus).timer.timeout_flag == MODBUS_TIMER_NOTIMEOUT)||((*modbus).timer.timeout_flag == MODBUS_TIMER_BYTE_OUT))\
						&& ((*modbus).error_info == MODBUS_NO_ERROR)); //����Ϊ�˱�֤ʵʱ��ʹ��ѭ����ѯ
					
					if(((*modbus).error_info == MODBUS_BUF_FULL) || ((*modbus).error_info == MODBUS_BYTE_TIME_OUT))
					{
       Modbus_Error_Slove(modbus);
						(*modbus).status = MODBUS_FREE;
						Reset_Timer(modbus);
						Clear_RX_Buf(modbus);
					}
					else
					{
						(*modbus).status = MODBUS_ANALYSIS;
						Reset_Timer(modbus);
					}
				}
				else if((*modbus).status == MODBUS_ANALYSIS)	/*�����׶η������ݸ�ʽ�Ƿ���ȷ�����ý�������������Ӧ����*/
				{
						BE_Check(modbus);						/*��֡��β����У��*/
						if((*modbus).error_info == MODBUS_NO_ERROR)
						{
							FC_Realized(modbus);				/*������У��ʵ��*/
						}
						else
						{
							(*modbus).status = MODBUS_FREE;
       (*modbus).error_info = MODBUS_NO_ERROR;
							Clear_RX_Buf(modbus);
						}
				}
		}
		else
		{
			/*Modbus�豸��ɫ���ô���*/
		}
	}
	else
	{
		/*����Modbusģʽ,������*/

	}

}
/******************************************************************************************************************
*                                   void Modbus_Default_Parameter(modbus_st* modbus)
*Description : ����Modbus�����в���
*Arguments   : modbus Modbus�������ľ��
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
void Modbus_SendData(modbus_st* modbus,uint16  Outlength)
{  
    uint8 i;
	uint16 temp;
	temp = Modbus_CRC16((*modbus).rx_buf,Outlength);
	(*modbus).tx_buf[Outlength] = temp&0xFF;
	(*modbus).tx_buf[Outlength+1] = (temp&0xFF00) >> 8;

	if((*modbus).config.dev_mode == MODBUS_SLAVER)
	{
		(*modbus).tx_buf[0] = (*modbus).config.dev_address;
	}

	for(i = 0;i<(Outlength + 2);i++)
	{
		MODBUS_SEND_BYTE((*modbus).tx_buf[i]);
	}

	(*modbus).status = MODBUS_TX;
	(*modbus).timer.count_mode = MODBUS_TIMER_TX_EN;
}



