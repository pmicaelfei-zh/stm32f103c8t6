/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dmaRingBuffer.h"
#include "rePrintf.h"
#include "shell_port.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_RX_TypeDef uart_rx_data_t[];
extern uint16_t uart_buff_ctrl ;
extern UART_HandleTypeDef huart1;

static char data     = 0;
static int  rcv_cnts = 0;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId main_taskHandle;
osThreadId uartRx_taskHandle;
osMessageQId rxQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void uart_rx_entry(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of rxQueue */
  osMessageQDef(rxQueue, 5, void *);
  rxQueueHandle = osMessageCreate(osMessageQ(rxQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityLow, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of main_task */
  osThreadDef(main_task, StartTask02, osPriorityNormal, 0, 128);
  main_taskHandle = osThreadCreate(osThread(main_task), NULL);

  /* definition and creation of uartRx_task */
  osThreadDef(uartRx_task, uart_rx_entry, osPriorityHigh, 0, 256);
  uartRx_taskHandle = osThreadCreate(osThread(uartRx_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the main_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_uart_rx_entry */
/**
* @brief Function implementing the uartRx_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uart_rx_entry */
void uart_rx_entry(void const * argument)
{
  /* USER CODE BEGIN uart_rx_entry */
   userShellInit();
	
   /*配置串口帧识别接收中断*/
   __HAL_UART_ENABLE_IT(&huart1 ,UART_IT_IDLE);
   HAL_UARTEx_ReceiveToIdle_DMA(&huart1 , uart_rx_data_t[uart_buff_ctrl].buffer,UART_BUFFER_SIZE);    
   UART_RX_TypeDef *RecvUartData;
	
  /* Infinite loop */
  for(;;)
  {
    if(xQueueReceive(rxQueueHandle,&RecvUartData,portMAX_DELAY )== pdTRUE)
    {
		  #if 0
        //函数调试内容
        //队列中存入了数据
		  //将数据做了一个串口回显，当然你用作其它处理也可以
		  //HAL_UART_Transmit(&huart1,RecvUartData->buffer,RecvUartData->size,1000);	
        #endif
       
        //此处需要对收到的字符串进行命令解析
        //此处需要按字节顺序对接收到的字符进行解析
       
       if( RecvUartData->size > 0 )
       { //有数据到达
          //接收计数器在使用前，清空为0
          rcv_cnts = 0;
          while( rcv_cnts < RecvUartData->size )
          {
             shellTask(&shell,(RecvUartData->buffer)[rcv_cnts]);
             rcv_cnts ++; //将索引增加一个字节
          }
       }       
	 }  
    //osDelay(1);
  }
  /* USER CODE END uart_rx_entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

