/*
 * PollingRoutine.c
 *
 *  Created on: Dec 17, 2022
 *      Author: codek
 */

#include "main.h"
#include "PollingRoutine.h"

extern UART_HandleTypeDef huart2;

UART_QueueStruct txMsg = {0};

uint32_t someState;


//Drive Mode
uint8_t EcoON[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x10, 0x55, 0x01, 0xBA };
uint8_t NormalON[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x10, 0x55, 0x01, 0xBC };
uint8_t SportON[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x10, 0x55, 0x01, 0xBE };
uint8_t modeClear[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x10, 0x55, 0x00, 0x00 };


// is called before main while loop
void PollingInit(void)
{
	someState = 1; // send EconON
}

// main while loop
void PollingRoutine(void)
{
	SomeTask();
	UART_SendMessage();
}

// basic example of how to save data to txMsgQueue queue buffer
void SomeTask(void)
{
	int status = 0;

	switch(someState)
	{
	case 1:
		status= SaveToUART_TxBuffer(EcoON, sizeof(EcoON));
		break;
	case 2:
		status = SaveToUART_TxBuffer(NormalON, sizeof(NormalON));
		break;
	case 3:
		status = SaveToUART_TxBuffer(SportON, sizeof(SportON));
		break;
	case 4:
		status = SaveToUART_TxBuffer(modeClear, sizeof(modeClear));
		break;
	}

	if(status != 0)
	{
		// buffer is full. Use some notification so user is aware and can increase UART_TX_QUEUE_SIZE
	}

	someState = 0;
}

int SaveToUART_TxBuffer(uint8_t *data, uint32_t dataSize)
{
	if(txMsg.RING_BUFF.ptr.iCnt_OverFlow)
	{
		return 1; // Overflow. Increase UART_TX_QUEUE_SIZE
	}
	memcpy(&txMsg.QUEUE.msg[txMsg.RING_BUFF.ptr.iIndexIN].data, data, dataSize);
	txMsg.QUEUE.msg[txMsg.RING_BUFF.ptr.iIndexIN].dataSize = dataSize;
	DRV_RingBuffPtr__Input(&txMsg.RING_BUFF.ptr, UART_TX_QUEUE_SIZE);

	return 0; // no error
}

void UART_SendMessage(void)
{
	if(txMsg.RING_BUFF.ptr.iCnt_Handle)
	{
		if(HAL_UART_Transmit_IT(&huart2, txMsg.QUEUE.msg[txMsg.RING_BUFF.ptr.iIndexOUT].data, txMsg.QUEUE.msg[txMsg.RING_BUFF.ptr.iIndexOUT].dataSize) == HAL_OK)
		{
			// transmit is good, increment queue pointer to next available message
			DRV_RingBuffPtr__Output(&txMsg.RING_BUFF.ptr, UART_TX_QUEUE_SIZE);
		}
	}
}

