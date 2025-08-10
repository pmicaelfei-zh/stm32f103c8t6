/* 带DMA的增强版 */
#include<FreeRTOS.h>
#include<stdarg.h>
#include<stdio.h>
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "portmacro.h"

static uint8_t buf[256];
void uart_printf(const char *format, ...) {
   
    if ( xPortIsInsideInterrupt() == pdTRUE )
      portDISABLE_INTERRUPTS();
    else
      portENTER_CRITICAL(); 
    
    va_list args;
    va_start(args, format);
    int len = vsnprintf((char*)buf, sizeof(buf), format, args);
    va_end(args);
    
    while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX){}     
    HAL_UART_Transmit(&huart1, buf, len,100);
    
    if ( xPortIsInsideInterrupt() == pdTRUE )
      portENABLE_INTERRUPTS();
    else
      portEXIT_CRITICAL(); 
   
    return ;
}

