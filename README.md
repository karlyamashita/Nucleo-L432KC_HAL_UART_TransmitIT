# Creating a UART Tx ring buffer queue

###### - Development board is a Nucleo-L432KC
###### - STM32CubeIDE 1.11.00
###### - Using HAL_UART_Transmit_IT but HAL_UART_Transmit_DMA could be used as well.


The issue that usually happens is that if a developer has only one array that they pass to HAL_UART_Transmit_IT, the data could be updated with new data while the HAL is transmitting the current data. And if there is a checksum involved then that packet will fail the checksum verification. 

Having a queue to hold several packets will allow new data to be placed into the queue without affecting the data that is currently being transmitted.

This also demonstrates how to act upon the HAL return status. Majority of newbies look at the examples that ST has. The examples are half baked code. Upon a non HAL_OK return status they call Error_Handler() which is useless. Most people decide not to check for HAL_OK which is the cause for most issues. 

