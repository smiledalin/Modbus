/*
******************************************************************************************************************
*Filename      :Modbus_RTU_Slaver_FC_V1.0.c
*Programmer(s) :Nrush
*Description   :该文件为Modbus RTU从机通讯功能码实现代码与硬件平台无关，已实现的功能码
                1,2,3,4,5,6,15,16
******************************************************************************************************************
*/
/* $ PAGE*/
#include "Modbus_RTU_Slaver_port_V1.0.h"

#if MODBUS_FC01 == 1
/******************************************************************************************************************
*                            void Modbus_FC01_SRX(void* ptr)
*Description : 01功能码读保持线圈实现函数
*Arguments   : ptr传入参数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static modbus_fc_list_st modbus_FC01;

void Modbus_FC01_SRX(void* ptr)
{
	modbus_st *modbus = (modbus_st*)ptr;
	uint16 address = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[2],(*modbus).rx_buf[3]);
	uint16 num = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[4],(*modbus).rx_buf[5]);

	Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
	Modbus_Reset_Slaver(modbus);
   (*modbus).rx_buf[1] = 0x01;
   
#if  MODBUS_HOLDBIT_REG_START > 0  
	
	if((address + num) > (MODBUS_HOLDBIT_REG_START+MODBUS_HOLDBIT_REG_MAX_LENGTH)\
      || (address < MODBUS_HOLDBIT_REG_START))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#else
   if((address + num) > (MODBUS_HOLDBIT_REG_START+MODBUS_HOLDBIT_REG_MAX_LENGTH))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#endif
	else
	{
      if((num <= MODBUS_MAX_RW_BITS)   && (num != 0))
      {
         if(Modbus_Read_HoldBit_Data(address,num,&(*modbus).rx_buf[3]) == MODBUS_OFF)
         {
            if(num%8)
            {
               (*modbus).rx_buf[2] = num/8 + 1;
            }
            else
            {
               (*modbus).rx_buf[2] = num/8;
            }
            Modbus_SendData(modbus,(*modbus).rx_buf[2] + 3);
         }
         else
         {
            (*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_4;
         }
      }
      else
      {
         (*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_3;
      }
         
	}
	
	if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
      
   Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);   
}
#endif

#if MODBUS_FC02 == 1
/******************************************************************************************************************
*                            void Modbus_FC02_SRX(void* ptr)
*Description : 02功能码读输入线圈实现函数
*Arguments   : ptr传入参数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static modbus_fc_list_st modbus_FC02;

void Modbus_FC02_SRX(void* ptr)
{
	modbus_st *modbus = (modbus_st*)ptr;
	uint16 address = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[2],(*modbus).rx_buf[3]);
	uint16 num = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[4],(*modbus).rx_buf[5]);

	Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
	Modbus_Reset_Slaver(modbus);
   (*modbus).rx_buf[1] = 0x02;
	
#if  MODBUS_INBIT_REG_START > 0  
	
	if((address + num - 1) > (MODBUS_INBIT_REG_START+MODBUS_INBIT_REG_MAX_LENGTH)\
      || (address < MODBUS_INBIT_REG_START))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#else
   if((address + num - 1) > (MODBUS_INBIT_REG_START+MODBUS_INBIT_REG_MAX_LENGTH))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#endif
	else
	{
      if((num <= MODBUS_MAX_RW_BITS)  && (num != 0))
      {
            if(Modbus_Read_InBit_Data(address,num,&(*modbus).rx_buf[3]) == MODBUS_OFF)
            {
            if(num%8)
            {
               (*modbus).rx_buf[2] = num/8 + 1;
            }
            else
            {
               (*modbus).rx_buf[2] = num/8;
                  }
                  Modbus_SendData(modbus,(*modbus).rx_buf[2] + 3);
            }
            else
            {
               (*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_4;
            }
      }
      else
      {
         (*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_3;
      }
		
	}
	
	if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
      
   Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);
   
}
#endif

#if MODBUS_FC03 == 1
/******************************************************************************************************************
*                            void Modbus_FC03_SRX(void* ptr)
*Description : 03功能码读保持寄存器实现函数
*Arguments   : ptr传入参数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static modbus_fc_list_st modbus_FC03;
void Modbus_FC03_SRX(void* ptr)
{
	modbus_st *modbus = (modbus_st*)ptr;
	uint16 address = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[2],(*modbus).rx_buf[3]);
	uint16 num = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[4],(*modbus).rx_buf[5]);
	
	Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
	
	Modbus_Reset_Slaver(modbus);
   (*modbus).rx_buf[1] = 0x03;
   
#if  MODBUS_HOLDBYTE_REG_START > 0  	
	if((address + num) > (MODBUS_HOLDBYTE_REG_START+MODBUS_HOLDBYTE_REG_MAX_LENGTH)\
      || (address < MODBUS_HOLDBYTE_REG_START))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#else
   if((address + num) > (MODBUS_HOLDBYTE_REG_START+MODBUS_HOLDBYTE_REG_MAX_LENGTH))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#endif
	else
	{
		if((num <= MODBUS_MAX_RW_2BYTES) && (num != 0))
		{
			if(Modbus_Read_Hold2Bytes_Data(address,num,&(*modbus).rx_buf[3]) == MODBUS_OFF)
			{
				(*modbus).rx_buf[2] = num*2;
				Modbus_SendData(modbus,(*modbus).rx_buf[2] + 3);
			}
			else
			{
				(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_4;
			}
		}
		else
		{
			(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_3;
		}
		
	}
	
	if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
	
	Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);	
}
#endif

#if MODBUS_FC04 == 1
/******************************************************************************************************************
*                            void Modbus_FC04_SRX(void* ptr)
*Description : 04功能码读输入寄存器实现函数
*Arguments   : ptr传入参数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static modbus_fc_list_st modbus_FC04;
void Modbus_FC04_SRX(void* ptr)
{
	modbus_st *modbus = (modbus_st*)ptr;
	uint16 address = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[2],(*modbus).rx_buf[3]);
	uint16 num = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[4],(*modbus).rx_buf[5]);
	
	Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
	
	Modbus_Reset_Slaver(modbus);
   (*modbus).rx_buf[1] = 0x04;
   
#if  MODBUS_INBYTE_REG_START > 0  	
	if((address + num) > (MODBUS_INBYTE_REG_START+MODBUS_INBYTE_REG_MAX_LENGTH)\
      || (address < MODBUS_INBYTE_REG_START))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#else
   if((address + num) > (MODBUS_INBYTE_REG_START+MODBUS_INBYTE_REG_MAX_LENGTH))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#endif
	else
	{
		if((num <= MODBUS_MAX_RW_2BYTES) && (num != 0))
		{
			if(Modbus_Read_In2Bytes_Data(address,num,&(*modbus).rx_buf[3]) == MODBUS_OFF)
			{
				(*modbus).rx_buf[2] = num*2;
				Modbus_SendData(modbus,(*modbus).rx_buf[2] + 3);
			}
			else
			{
				(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_4;
			}
		}
		else
		{
			(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_3;
		}
		
	}
	
	if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
	
	Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);	
}
#endif

#if MODBUS_FC05 == 1
/******************************************************************************************************************
*                            void Modbus_FC05_SRX(void* ptr)
*Description : 05功能码写单个保持线圈实现函数
*Arguments   : ptr传入参数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static modbus_fc_list_st modbus_FC05;
void Modbus_FC05_SRX(void* ptr)
{
	modbus_st *modbus = (modbus_st*)ptr;
	uint16 address = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[2],(*modbus).rx_buf[3]);
	uint16 value = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[4],(*modbus).rx_buf[5]);
	uint8 temp = 0;
   
	Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
   
#if  MODBUS_HOLDBIT_REG_START > 0  	
	if((address) > (MODBUS_HOLDBIT_REG_START+MODBUS_HOLDBIT_REG_MAX_LENGTH - 1)\
      || (address < MODBUS_HOLDBIT_REG_START))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#else
   if((address) > (MODBUS_HOLDBIT_REG_START+MODBUS_HOLDBIT_REG_MAX_LENGTH - 1))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#endif
   else
   {
      if((value == 0xFF00) || (value == 0x0000))
		{
         if(value == 0xFF00)
         {
            temp = 0x01;
         }
         else
         {
            temp = 0x00;
         }
         
			if(Modbus_Write_HoldBit_Data(address,1,&temp) == MODBUS_OFF)
			{
				Modbus_SendData(modbus,6);
			}
			else
			{
				(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_4;
			}
		}
		else
		{
			(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_3;
		}
   }
         
		if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
   Modbus_Reset_Slaver(modbus);
	Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);	
}
#endif

#if MODBUS_FC06 == 1
/******************************************************************************************************************
*                            void Modbus_FC06_SRX(void* ptr)
*Description : 06功能码写单个保持寄存器实现函数
*Arguments   : ptr传入参数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static modbus_fc_list_st modbus_FC06;
void Modbus_FC06_SRX(void* ptr)
{
	modbus_st *modbus = (modbus_st*)ptr;
	uint16 address = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[2],(*modbus).rx_buf[3]);
   
	Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
   
#if  MODBUS_HOLDBYTE_REG_START > 0  	
	if((address) > (MODBUS_HOLDBYTE_REG_START+MODBUS_HOLDBYTE_REG_MAX_LENGTH - 1)\
      || (address < MODBUS_HOLDBYTE_REG_START))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#else
   if((address) > (MODBUS_HOLDBYTE_REG_START+MODBUS_HOLDBYTE_REG_MAX_LENGTH - 1))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#endif
   else
   {      
			if(Modbus_Write_Hold2Bytes_Data(address,1,&(*modbus).rx_buf[4]) == MODBUS_OFF)
			{
				Modbus_SendData(modbus,6);
			}
			else
			{
				(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_4;
			}
   }
         
		if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
   Modbus_Reset_Slaver(modbus);
	Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);	
}
#endif

#if MODBUS_FC15 == 1
/******************************************************************************************************************
*                            void Modbus_FC15_SRX(void* ptr)
*Description : 15功能码写多个保持线圈实现函数
*Arguments   : ptr传入参数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static modbus_fc_list_st modbus_FC15;
void Modbus_FC15_SRX(void* ptr)
{
	modbus_st *modbus = (modbus_st*)ptr;
	uint16 address = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[2],(*modbus).rx_buf[3]);
   uint16 num = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[4],(*modbus).rx_buf[5]);
   
	Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
   (*modbus).rx_buf[1] = 15;
   
#if  MODBUS_HOLDBIT_REG_START > 0  	
	if((address + num) > (MODBUS_HOLDBIT_REG_START+MODBUS_HOLDBIT_REG_MAX_LENGTH)\
      || (address < MODBUS_HOLDBIT_REG_START))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#else
   if((address + num) > (MODBUS_HOLDBIT_REG_START+MODBUS_HOLDBIT_REG_MAX_LENGTH))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#endif
   else
   {      
	   if((num <= MODBUS_MAX_RW_BITS)  && (num != 0))
      {
            if(Modbus_Write_HoldBit_Data(address,num,&(*modbus).rx_buf[7]) == MODBUS_OFF)
            {
                 (*modbus).rx_buf[2] = MODBUS_HIGH_BYTE(address);
                 (*modbus).rx_buf[3] = MODBUS_LOW_BYTE(address);
                 (*modbus).rx_buf[4] = MODBUS_HIGH_BYTE(num);
                 (*modbus).rx_buf[5] = MODBUS_LOW_BYTE(num);
                 Modbus_SendData(modbus,6);
            }
            else
            {
               (*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_4;
            }
      }
      else
      {
         (*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_3;
      }
   }
         
		if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
 
   Modbus_Reset_Slaver(modbus);
	Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);	
}
#endif

#if MODBUS_FC16 == 1
/******************************************************************************************************************
*                            void Modbus_FC16_SRX(void* ptr)
*Description : 16功能码写多个保持寄存器实现函数
*Arguments   : ptr传入参数
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/
static modbus_fc_list_st modbus_FC16;
void Modbus_FC16_SRX(void* ptr)
{
	modbus_st *modbus = (modbus_st*)ptr;
	uint16 address = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[2],(*modbus).rx_buf[3]);
	uint16 num = MODBUS_COMBINATION_BYTES((*modbus).rx_buf[4],(*modbus).rx_buf[5]);
	
	Modbus_Usart_IRQ_Disable_Port((*modbus).usart_reg);
	
   (*modbus).rx_buf[1] = 16;
   
#if  MODBUS_HOLDBYTE_REG_START > 0  	
	if((address + num) > (MODBUS_HOLDBYTE_REG_START+MODBUS_HOLDBYTE_REG_MAX_LENGTH)\
      || (address < MODBUS_HOLDBYTE_REG_START))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#else
   if((address + num) > (MODBUS_HOLDBYTE_REG_START+MODBUS_HOLDBYTE_REG_MAX_LENGTH))
	{
		(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_2;
	}
#endif
	else
	{
		if((num <= MODBUS_MAX_RW_2BYTES) && (num != 0))
		{
			if(Modbus_Write_Hold2Bytes_Data(address,num,&(*modbus).rx_buf[7]) == MODBUS_OFF)
			{
				(*modbus).rx_buf[2] = MODBUS_HIGH_BYTE(address);
            (*modbus).rx_buf[3] = MODBUS_LOW_BYTE(address);
            (*modbus).rx_buf[4] = MODBUS_HIGH_BYTE(num);
            (*modbus).rx_buf[5] = MODBUS_LOW_BYTE(num);
				Modbus_SendData(modbus,6);
			}
			else
			{
				(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_4;
			}
		}
		else
		{
			(*modbus).error_info = MODBUS_RESPONSE_ABNORMAL_3;
		}
		
	}
	
	if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		Modbus_Error_Slove(modbus);
	}
	
   Modbus_Reset_Slaver(modbus);
	Modbus_Usart_IRQ_Enable_Port((*modbus).usart_reg);	
}
#endif

/******************************************************************************************************************
*                           void Modbus_List_Init(modbus_st* modbus)
*Description : 初始化Modbus功能码实现函数列表
*Arguments   : modbus Modbus控制器的句柄
*Returns     : None
*Notes       : None
*******************************************************************************************************************
*/

void Modbus_List_Init(modbus_st* modbus)	
{
#if MODBUS_FC01 == 1
	Modbus_Add_FC_List_Item(modbus,&modbus_FC01,1,Modbus_FC01_SRX);
#endif
#if MODBUS_FC02 == 1	
	Modbus_Add_FC_List_Item(modbus,&modbus_FC02,2,Modbus_FC02_SRX);
#endif
#if MODBUS_FC03 == 1	
   Modbus_Add_FC_List_Item(modbus,&modbus_FC03,3,Modbus_FC03_SRX);
#endif
#if MODBUS_FC04 == 1	
   Modbus_Add_FC_List_Item(modbus,&modbus_FC04,4,Modbus_FC04_SRX);
#endif
#if MODBUS_FC05 == 1	
   Modbus_Add_FC_List_Item(modbus,&modbus_FC05,5,Modbus_FC05_SRX);
#endif
#if MODBUS_FC06 == 1	
   Modbus_Add_FC_List_Item(modbus,&modbus_FC06,6,Modbus_FC06_SRX);
#endif
#if MODBUS_FC15 == 1	
   Modbus_Add_FC_List_Item(modbus,&modbus_FC15,15,Modbus_FC15_SRX);
#endif
#if MODBUS_FC16 == 1	
   Modbus_Add_FC_List_Item(modbus,&modbus_FC16,16,Modbus_FC16_SRX);
#endif
   Modbus_User_FCList_Init_Port(modbus);
}


