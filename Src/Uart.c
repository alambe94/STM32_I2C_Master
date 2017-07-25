/*
 * Uart.c
 *
 *  Created on: 10-Jul-2017
 *      Author: medprime
 */

#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>



extern UART_HandleTypeDef huart1;


void Uart_print(char* s)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)s,strlen(s),10);
}

void Uart_println(char* s)
{
	uint8_t tmp='\n';
	HAL_UART_Transmit(&huart1,(uint8_t*)s,strlen(s),10);
	HAL_UART_Transmit(&huart1, &tmp,1,10);

}


void Uart_print_numb(int data)
{
	char tmp[10];
	itoa(data,tmp,10);
	HAL_UART_Transmit(&huart1,(uint8_t*)tmp,strlen(tmp),10);
}

void Uart_print_numbln(int data)
{
	char tmp1[10];
	uint8_t tmp2='\n';
	itoa(data,tmp1,10);
	HAL_UART_Transmit(&huart1,(uint8_t*)tmp1,strlen(tmp1),10);
	HAL_UART_Transmit(&huart1, &tmp2,1,10);

}


