//
// Created by 26808 on 24-8-27.
//

#ifndef UART_DEVICE_H
#define UART_DEVICE_H

#include "windows.h"

class UartDevice{
protected:
	int byteNumRead;
	int byteNumWritten;

	HANDLE comHandle;
	DCB dcbParams;
	COMMTIMEOUTS timeouts;
public:
	char* portName;
	unsigned baudRate;

	int connect();
	int write(char* buffer, unsigned bufferSize);
	int read(char* buffer, unsigned bufferSize);

	UartDevice();
	UartDevice(char* portName, unsigned baudRate);
	void setParams(char* portName, unsigned baudRate);
	~UartDevice();

};

static UartDevice uartDevice;
#endif //UART_DEVICE_H
