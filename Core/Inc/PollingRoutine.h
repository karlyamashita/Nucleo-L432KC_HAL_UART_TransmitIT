/*
 * PollingRoutine.h
 *
 *  Created on: Dec 17, 2022
 *      Author: codek
 */

#ifndef INC_POLLINGROUTINE_H_
#define INC_POLLINGROUTINE_H_


#define UART_TX_DATA_SIZE 8
#define UART_TX_QUEUE_SIZE 4

typedef struct
{
	uint8_t uartPort;
	uint8_t data[UART_TX_DATA_SIZE];
	uint8_t dataSize;
}UART_Data;

typedef struct
{
	struct
	{
		UART_Data msg[UART_TX_QUEUE_SIZE];
	}QUEUE;
	struct
	{
		RING_BUFF_INFO ptr;
	}RING_BUFF;
}UART_QueueStruct;

void PollingInit(void);
void PollingRoutine(void);
void SomeTask(void);

int SaveToUART_TxBuffer(uint8_t *data, uint32_t dataSize);
void UART_SendMessage(void);



#endif /* INC_POLLINGROUTINE_H_ */
