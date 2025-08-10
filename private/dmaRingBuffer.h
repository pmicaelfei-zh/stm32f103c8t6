#ifndef __H_DMARINGBUFFER__
#define __H_DMARINGBUFFER__

#include "FreeRTOS.h"

#define UART_BUFFER_SIZE 128
#define UART_BUFFER_COUNT 2

typedef struct {
	uint8_t buffer[UART_BUFFER_SIZE];	
	uint16_t size;	
}UART_RX_TypeDef;


#endif 