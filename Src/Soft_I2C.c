#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "Soft_I2C.h"
#include "Uart.h"

void Soft_I2C_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;

	GPIO_InitStruct.Pin = SDA | SCL;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(Soft_I2C_Port, &GPIO_InitStruct);

	SDA_H;
	SCL_H;

}

_Bool Soft_I2C_Start(void) {
	SDA_H;
	Soft_I2C_Delay(100);
	SCL_H;
	Soft_I2C_Delay(100);
	if (!SDA_R) {
		return Soft_I2C_Err;
	}

	SDA_L;

	Soft_I2C_Delay(100);
	if (SDA_R) {
		return Soft_I2C_Err;
	}

	Soft_I2C_Delay(100);
	return Soft_I2C_OK;

}

void Soft_I2C_Stop(void) {
	SCL_L;
	Soft_I2C_Delay(100);
	SDA_L;
	Soft_I2C_Delay(100);
	SCL_H;
	Soft_I2C_Delay(100);
	SDA_H;
	Soft_I2C_Delay(100);
}

_Bool Soft_I2C_WaitAck(unsigned int timeOut) {
	SCL_L;
	Soft_I2C_Delay(100);
	SDA_H;
	Soft_I2C_Delay(100);
	SCL_H;
	Soft_I2C_Delay(100);

	while (SDA_R) {
		if (!(--timeOut)) {
			//Uart_println("WaitAck TimeOut\r");

			Soft_I2C_Stop();

			return Soft_I2C_Err;
		}
	}

	SCL_L;
	Soft_I2C_Delay(100);

	return Soft_I2C_OK;

}

void Soft_I2C_Ack(void) {

	SCL_L;
	Soft_I2C_Delay(100);
	SDA_L;
	Soft_I2C_Delay(100);
	SCL_H;
	Soft_I2C_Delay(100);
	SCL_L;
	Soft_I2C_Delay(100);

}

void Soft_I2C_NAck(void) {

	SCL_L;
	Soft_I2C_Delay(10);
	SDA_H;
	Soft_I2C_Delay(10);
	SCL_H;
	Soft_I2C_Delay(100);
	SCL_L;
	Soft_I2C_Delay(100);

}

void Soft_I2C_SendByte(unsigned char byte) {

	unsigned char count = 8;

    SCL_L;
    Soft_I2C_Delay(100);
    while(count--)
    {
		if(byte & 0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}

		byte <<= 1;

        Soft_I2C_Delay(100);
		SCL_H;
        Soft_I2C_Delay(100);
		SCL_L;
    }

}

unsigned char Soft_I2C_RecvByte(void) {

    uint8_t i = 8;
    uint8_t ReceivedByte = 0;

    SDA_H;
    while(i--)
    {
      ReceivedByte<<=1;
      SCL_L;
		Soft_I2C_Delay(100);
	    SCL_H;
		Soft_I2C_Delay(100);
      if(SDA_R)
      {
        ReceivedByte |= 0x01;
      }
    }
    SCL_L;
    return ReceivedByte;

}

_Bool Soft_I2C_WriteByte(unsigned char slaveAddr, unsigned char regAddr,
		unsigned char *byte) {

	unsigned char addr = 0;

	addr = slaveAddr << 1;

	Soft_I2C_Start();

	Soft_I2C_SendByte(addr);
	if (Soft_I2C_WaitAck(5000)) {
		return Soft_I2C_Err;
		Soft_I2C_Stop();
	} else {
		Soft_I2C_SendByte(regAddr);
		if (Soft_I2C_WaitAck(5000)) {
			return Soft_I2C_Err;
			Soft_I2C_Stop();
		}
	}

	if (byte) {
		Soft_I2C_SendByte(*byte);
		if (Soft_I2C_WaitAck(5000)) {
			return Soft_I2C_Err;
			Soft_I2C_Stop();
		}
	}

	Soft_I2C_Stop();

	return Soft_I2C_OK;

}

_Bool Soft_I2C_ReadByte(unsigned char slaveAddr, unsigned char regAddr,
		unsigned char *val) {

	unsigned char addr = 0;

	addr = slaveAddr << 1;

	Soft_I2C_Start();

	Soft_I2C_SendByte(addr);
	if (Soft_I2C_WaitAck(5000))
		return Soft_I2C_Err;

	Soft_I2C_SendByte(regAddr);
	if (Soft_I2C_WaitAck(5000))
		return Soft_I2C_Err;

	Soft_I2C_Start();

	Soft_I2C_SendByte(addr + 1);
	if (Soft_I2C_WaitAck(5000))
		return Soft_I2C_Err;

	*val = Soft_I2C_RecvByte();
	Soft_I2C_NAck();

	Soft_I2C_Stop();

	return Soft_I2C_OK;

}

_Bool Soft_I2C_WriteBytes(unsigned char slaveAddr, unsigned char regAddr,
		unsigned char *buf, unsigned char num) {

	unsigned char addr = 0;

	addr = slaveAddr << 1;

	Soft_I2C_Start();

	Soft_I2C_SendByte(addr);
	if (Soft_I2C_WaitAck(5000))
		return Soft_I2C_Err;

	Soft_I2C_SendByte(regAddr);
	if (Soft_I2C_WaitAck(5000))
		return Soft_I2C_Err;

	while (num--) {
		Soft_I2C_SendByte(*buf);
		if (Soft_I2C_WaitAck(5000))
			return Soft_I2C_Err;

		buf++;

		Soft_I2C_Delay(100);
	}

	Soft_I2C_Stop();

	return Soft_I2C_OK;

}

_Bool Soft_I2C_ReadBytes(unsigned char slaveAddr, unsigned char regAddr,
		unsigned char *buf, unsigned char num) {

	unsigned short addr = 0;

	addr = slaveAddr << 1;

	Soft_I2C_Start();

	Soft_I2C_SendByte(addr);
	if (Soft_I2C_WaitAck(5000))
		return Soft_I2C_Err;

	Soft_I2C_SendByte(regAddr);
	if (Soft_I2C_WaitAck(5000))
		return Soft_I2C_Err;

	Soft_I2C_Start();

	Soft_I2C_SendByte(addr + 1);
	if (Soft_I2C_WaitAck(5000))
		return Soft_I2C_Err;

	while (num--) {
		*buf = Soft_I2C_RecvByte();
		buf++;

		if (num == 0) {
			Soft_I2C_NAck();
		} else {
			Soft_I2C_Ack();
		}
	}

	Soft_I2C_Stop();

	return Soft_I2C_OK;

}

void Soft_I2C_Delay(unsigned int delay) {
	unsigned int i = 30;
	//unsigned int i=delay;
	while (i--)
		;

}
