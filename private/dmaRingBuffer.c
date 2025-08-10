#include "dmaRingBuffer.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "stm32f1xx_hal.h"

UART_RX_TypeDef uart_rx_data_t[UART_BUFFER_COUNT];
uint16_t uart_buff_ctrl = 0;

extern osMessageQId rxQueueHandle;



void USART1_DMAHandler(UART_HandleTypeDef *hdl,uint16_t pos)
{
   
   UART_RX_TypeDef *Data;//再定义一个这个结构体，包含的东西与uart_rx_data_t一致，我们要将uart_rx_data_t中的数据传到Data指针当中再一个一个传输
   
   HAL_UART_DMAStop(hdl );//停止DMA传输，关闭DMA
   
   uart_rx_data_t[uart_buff_ctrl].size = pos;  
   
   
   Data = &uart_rx_data_t[uart_buff_ctrl];   
   
   xQueueSendFromISR(rxQueueHandle,&Data,NULL);
   
   uart_buff_ctrl++;//自加
   
   uart_buff_ctrl %= UART_BUFFER_COUNT; //再取余，让它计到5时重新归零，当然这里也可以直接使用if语句
   
   HAL_UARTEx_ReceiveToIdle_DMA(hdl , uart_rx_data_t[uart_buff_ctrl].buffer,UART_BUFFER_SIZE);//重新开启DMA传输，传输buffer。这个函数在任务当中也要用到		
   
}
