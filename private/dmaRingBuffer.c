#include "dmaRingBuffer.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "stm32f1xx_hal.h"

UART_RX_TypeDef uart_rx_data_t[UART_BUFFER_COUNT];
uint16_t uart_buff_ctrl = 0;

extern osMessageQId rxQueueHandle;



void USART1_DMAHandler(UART_HandleTypeDef *hdl,uint16_t pos)
{
   
   UART_RX_TypeDef *Data;//�ٶ���һ������ṹ�壬�����Ķ�����uart_rx_data_tһ�£�����Ҫ��uart_rx_data_t�е����ݴ���Dataָ�뵱����һ��һ������
   
   HAL_UART_DMAStop(hdl );//ֹͣDMA���䣬�ر�DMA
   
   uart_rx_data_t[uart_buff_ctrl].size = pos;  
   
   
   Data = &uart_rx_data_t[uart_buff_ctrl];   
   
   xQueueSendFromISR(rxQueueHandle,&Data,NULL);
   
   uart_buff_ctrl++;//�Լ�
   
   uart_buff_ctrl %= UART_BUFFER_COUNT; //��ȡ�࣬�����Ƶ�5ʱ���¹��㣬��Ȼ����Ҳ����ֱ��ʹ��if���
   
   HAL_UARTEx_ReceiveToIdle_DMA(hdl , uart_rx_data_t[uart_buff_ctrl].buffer,UART_BUFFER_SIZE);//���¿���DMA���䣬����buffer�����������������ҲҪ�õ�		
   
}
