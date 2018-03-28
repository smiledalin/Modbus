#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "Modbus_RTU_Slaver_port_V1.0.h"
 int main(void)
 {	 
	  delay_init();	    	 //延时函数初始化	  
	  NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
     Modbus_Init(&modbus);
  	  while(1) 
	  {        
         Modbus_Receive_Data(&modbus);
	  }			
 }

