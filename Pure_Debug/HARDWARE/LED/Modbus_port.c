#include "Modbus_port.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys.h"


void Timer2(modbus_timer_st timer)
{
 int8 pre;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;

 
 pre = (((uint32)36000)/timer.tick_per_millisecond)-1;
 if(pre < 0)
 {
   pre = 0;
 }

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period = modbus.config.frame_space_time; 
	TIM_TimeBaseStructure.TIM_Prescaler = pre; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
 
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
 TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
 TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
 
 TIM_SetCompare1(TIM2,modbus.config.byte_space_time);

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

TIM_ClearITPendingBit(TIM2,TIM_IT_CC1); 
TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE ); 
TIM_Cmd(TIM2, DISABLE);					 	
}

void TIM2_IRQHandler(void)
{
 	 if(TIM_GetITStatus(TIM2,TIM_IT_CC1)==SET) 
	{
  modbus.timer.timeout_flag = MODBUS_TIMER_BYTE_OUT;
  TIM_ClearITPendingBit(TIM2,TIM_IT_CC1); 
	}
 else if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) 
	{
  modbus.timer.timeout_flag = MODBUS_TIMER_FRAME_OUT;
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	} 
}



void Modbus_Timer_Init(modbus_timer_st timer)
{
  Timer2(timer);
}

void Modbus_Usart_Init(modbus_usart_st usart)
{
   uart_init(115200);
}

//sample

static modbus_fc_list_st Read_Bit_FC1;

void Read_Bit_MTX(modbus_st* modbus,uint8 add)
{
	uint8 i = 0;
	(*modbus).tx_buf[i++] = add;		//从设备地址
	(*modbus).tx_buf[i++] = 0x01;		//命令号

	(*modbus).tx_buf[i++] = 0x02;		//命令其它内容


	/*发送数据*/
	Modbus_SendData(modbus,i);

	/*等待处理结束*/
	while(((*modbus).status != MODBUS_COMPLETE) && ((*modbus).error_info == MODBUS_NO_ERROR));

	/*检测是否发生错误*/
	if((*modbus).status == MODBUS_COMPLETE)
	{
		/*未发生错误*/
	}
	else if((*modbus).error_info != MODBUS_NO_ERROR)
	{
		/*发生错误*/
	}
	else
	{
		/*未知错误*/
	}
}

void Read_Bit_MRX(void* ptr)
{
	modbus_st* modbus = (modbus_st*) ptr;


	printf("FC:Read_Bit_MRX_OK");
	(*modbus).status = MODBUS_COMPLETE;
	//Clear_Buf(modbus);
}

void Read_Bit_SRX(void* ptr)
{
 modbus_st* modbus = (modbus_st*) ptr;
	printf("FC:Read_Bit_SRX_OK");
 (*modbus).status = MODBUS_FREE;
	Reset_Timer(modbus);
	Clear_RX_Buf(modbus);
 (*modbus).error_info = MODBUS_NO_ERROR;
}

static modbus_fc_list_st Read_InBit_FC2;
static modbus_fc_list_st Read_Regs_FC3;

void Modbus_List_Init(void)
{
	Modbus_Add_FC_List_Item(&modbus,&Read_Bit_FC1,0x01,Read_Bit_MRX,Read_Bit_SRX);
}

//void Modbus_Timer_Tick(void)
//{
//	if(modbus_state.status == MODBUS_RECEIVE)
//	{
//		modbus_timer_tick++;
//		if(modbus_timer_tick > MODBUS_FRAME_SPACE)
//		{
//			modbus_state.status = MODBUS_RECEIVE_COMPLETE;
//			modbus_timer_tick = 0;
//		}
//	}	
//}
//
//void Modbus_Receive_Data(uint8 data)
//{
//	modbus_state.status = MODBUS_NONE;
//	if(modbus_timer_tick < MODBUS_BYTE_SPACE)
//	{
//		modbus_timer_tick = 0;
//		modbus_rx_buf[modbus_state.receive_num] = data;
//		modbus_state.receive_num++;
//		modbus_state.status = MODBUS_RECEIVE;
//	}
//	else
//	{
//		modbus_state.status = MODBUS_TIME_OUT;
//	}
//}
//
//void Modbus_Analysis(void)
//{
//	if(modbus_state.status != MODBUS_RECEIVE_COMPLETE)
//}
//
//void Modbus_delay_bits(uint16 bits)
//{
//	delay_us(us_per_bit);	
//}
//
//void Modbus_delay_units(uint16 units)
//{
//	Modbus_delay_bits(MODBUS_UNIT_BITS*units);	
//}
//
//void Clear_Modbus_Status(void)
//{
//	uint8 i = 0;
//	for(i = 0;i<modbus_state.last_receive_nun;i++)
//	{
//		modbus_rx_buf[i] = 0;
//	}
//}
//
//
//
//uint8 Modbus_Check_Common_Response(uint8  *input,uint8  *rx,uint8 datalength)
//{  
//   unint16 temp = 0;
//   if((*(rx)) != (*input) return MODBUS_RESPONSE_ERR;
//   if((*(rx + 1)) == (*(input + 1))) 
//   {
//       temp = CRC16(modbus_rx_buf,modbus_state.receive_num - 2);
//	   
//	   if(((*(rx + modbus_state.receive_num - 2)) != HIGH_BYTE(temp)) |\
//	       ((*(rx + modbus_state.receive_num - 1)) != LOW_BYTE(temp)))
//		   {
//			   return  MODBUS_RESPONSE_CRCERR;
//		   }
//		   else
//		   {
//			   return  MODBUS_RESPONSE_OK;
//		   }
//   }
//   else if( ( (*(rx + 1)) == (0x80+(*(input + 1))) ) & (modbus_state.receive_num == 5) )
//   {
//	   temp = CRC16(modbus_rx_buf,modbus_state.receive_num - 2);
//	   
//	   if(((*(rx + modbus_state.receive_num - 2)) != HIGH_BYTE(temp)) |\
//	       ((*(rx + modbus_state.receive_num - 1)) != LOW_BYTE(temp)))
//		   {
//			   return  MODBUS_RESPONSE_CRCERR;
//		   }
//		   else
//		   {
//			   return  (*(rx + 2));
//		   }   
//   }
//   else
//   {
//	   return MODBUS_RESPONSE_ERR;
//   }  
//}
///******************************************************************************************************************
//*                                   void Read_Hold_Reg(unsigned char address,unsigned char len)
//*Description : 读保存寄存器
//*Arguments   : slave_adr:从机地址 address：寄存器地址 usDataLen：数据长度
//*Returns     : None
//*Notes       : None
//*******************************************************************************************************************
//*/
//
//uint8 Read_InBit(uint8 slave_adr,uint16 address,uint16 num)
//{
//  uint8  Txbuf[7];
//  uint16 temp;
//  unint8 cnt = 0;
//  unint16 delay_cnt = 0;
//  
//  Txbuf[cnt++] = slave_adr;
//  Txbuf[cnt++] = READ_IN_BIT; 
//  Txbuf[cnt++] = HIGH_BYTE(address);
//  Txbuf[cnt++] = LOW_BYTE(address);
//  Txbuf[cnt++] = HIGH_BYTE(num);
//  Txbuf[cnt++] = LOW_BYTE(num);
//  
//  temp = CRC16(Txbuf,cnt);
//  
//  Txbuf[cnt++] = HIGH_BYTE(temp);
//  Txbuf[cnt++] = LOW_BYTE(temp);
//  
//  temp = num+5;
//  modbus_state.receive_flag = MODBUS_NULL;
//  Modbus_SendData(Txbuf,cnt);
//  
//  Modbus_delay_units(temp);
//  
//  while(modbus_state.receive_flag == MODBUS_NULL)
//  {
//	 delay_cnt++;
//     Modbus_delay_bits(1);	 
//	 if(delay_cnt>MODBUS_MAX_DELAY)
//	 {
//		 return MODBUS_RESPONSE_TIMEOUT;
//	 }
//  }
//  
//  temp = Modbus_Check_Response(Txbuf,cnt);
//  return temp;
//}
//
///******************************************************************************************************************
//*        unsigned char Write_One_Reg(unsigned char slave_adr,unsigned int address,unsigned int data)
//*Description : 写单个寄存器
//*Arguments   : slave_adr:从机地址 address：寄存器地址 data：数据
//*Returns     : None
//*Notes       : None
//*******************************************************************************************************************
//*/
//unsigned char Write_One_Reg(unsigned char slave_adr,unsigned int address,int data)
//{
//  unsigned int CRC_Value=0;
//	char Check = FAIL;
//	unsigned int i=0;
//  Modbus_TX_Data_Buf[0] = slave_adr;
//  Modbus_TX_Data_Buf[1] = WRITE_ONE_REG;
//  Modbus_TX_Data_Buf[2] = address>>8;
//  Modbus_TX_Data_Buf[3] = address&0xFF;
//  Modbus_TX_Data_Buf[4] = data>>8;
//  Modbus_TX_Data_Buf[5] = data&0xFF;
//  
//  CRC_Value = CRC16(Modbus_TX_Data_Buf, 6);
//  
//  Modbus_TX_Data_Buf[6] = CRC_Value>>8;
//  Modbus_TX_Data_Buf[7] = CRC_Value&0xFF;
//  
//  Modbus_SendData(Modbus_TX_Data_Buf,8);
//  
//  Check = Modbus_Receive_Data(Modbus_RX_Data_Buf,8);
//	
//	if(Check == FAIL)
//	{
//		return FAIL;
//	}
//	
//  CRC_Value = CRC16(Modbus_RX_Data_Buf,6);
//  if((Modbus_RX_Data_Buf[6] != (CRC_Value>>8))\
//		|| (Modbus_RX_Data_Buf[7] != (CRC_Value&0xFF)))
//  	{
//  		return CRC_ERR;
//  	}
//	if((Modbus_RX_Data_Buf[0] != slave_adr)\
//		|| (Modbus_RX_Data_Buf[1] != WRITE_ONE_REG))
//  	{
//  		return FAIL;
//  	}
//	  for(i=0;i<10;i++)
//		{
//			Modbus_TX_Data_Buf[i]=0;
//			Modbus_RX_Data_Buf[i]=0;
//		}	
//  return SUCCESS;
//}







