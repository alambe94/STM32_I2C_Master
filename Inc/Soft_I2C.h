/*
 * Soft_I2C.h
 *
 *  Created on: 10-Jul-2017
 *      Author: medprime
 */

#ifndef SOFT_I2C_H_
#define SOFT_I2C_H_

#define Soft_I2C_OK		    0
#define Soft_I2C_Err		1

#define SDA		            GPIO_PIN_7
#define SCL		            GPIO_PIN_6
#define Soft_I2C_Port		GPIOB

//SDA		PB7
//SCL		PB6
#define SDA_H	HAL_GPIO_WritePin(Soft_I2C_Port, SDA,GPIO_PIN_SET)
#define SDA_L	HAL_GPIO_WritePin(Soft_I2C_Port, SDA,GPIO_PIN_RESET)
#define SDA_R	HAL_GPIO_ReadPin(Soft_I2C_Port, SDA)
#define SCL_R	HAL_GPIO_ReadPin(Soft_I2C_Port, SCL)


#define SCL_H	HAL_GPIO_WritePin(Soft_I2C_Port, SCL,GPIO_PIN_SET)
#define SCL_L	HAL_GPIO_WritePin(Soft_I2C_Port, SCL,GPIO_PIN_RESET)


void Soft_I2C_Init(void);

void Soft_I2C_SpeedCtl(unsigned short speed);

_Bool Soft_I2C_WriteByte(unsigned char slaveAddr, unsigned char regAddr,
		unsigned char *byte);

_Bool Soft_I2C_ReadByte(unsigned char slaveAddr, unsigned char regAddr,
		unsigned char *val);

_Bool Soft_I2C_WriteBytes(unsigned char slaveAddr, unsigned char regAddr,
		unsigned char *buf, unsigned char num);

_Bool Soft_I2C_ReadBytes(unsigned char slaveAddr, unsigned char regAddr,
		unsigned char *buf, unsigned char num);

_Bool Soft_I2C_Start(void);

void Soft_I2C_Stop(void);

_Bool Soft_I2C_WaitAck(unsigned int timeOut);

void Soft_I2C_Ack(void);

void Soft_I2C_NAck(void);

void Soft_I2C_SendByte(unsigned char byte);

void Soft_I2C_Delay(unsigned int delay);


unsigned char Soft_I2C_RecvByte(void);

#endif /* SOFT_I2C_H_ */
