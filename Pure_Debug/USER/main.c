#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "Modbus_RTU_Slaver_port_V1.0.h"
 int main(void)
 {	 
	  delay_init();	    	 //��ʱ������ʼ��	  
	  NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
     Modbus_Init(&modbus);
  	  while(1) 
	  {        
         Modbus_Receive_Data(&modbus);
	  }			
 }

