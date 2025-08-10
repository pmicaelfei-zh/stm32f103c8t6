/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
//#include "serial.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
//#include "cevent.h"
//#include "log.h"


Shell shell;
char shellBuffer[512];

//static SemaphoreHandle_t shellMutex;

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
static short userShellWrite(char *data, unsigned short len)
{
    //serialTransmit(&debugSerial, (uint8_t *)data, len, 0x1FF);
    HAL_UART_Transmit(&huart1,(unsigned char*)data,len,1000);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
static short userShellRead(char *data, unsigned short len)
{
    return 0;//serialReceive(&debugSerial, (uint8_t *)data, len, 0);
}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
static int userShellLock(Shell *shell)
{
    //xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
static int userShellUnlock(Shell *shell)
{
    //xSemaphoreGiveRecursive(shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    //shellMutex = xSemaphoreCreateMutex();

    shell.write =  userShellWrite;
    shell.read  =  0;//userShellRead;
   #if SHELL_USING_LOCK == 1
    shell.lock  =  userShellLock;
    shell.unlock = userShellUnlock;
   #endif
    shellInit(&shell, shellBuffer, 512);
//    if (xTaskCreate(shellTask, "shell", 256, &shell, 5, NULL) != pdPASS)
//    {
//        logError("shell task creat failed");
//    }
}


