//
// Created by 26808 on 24-8-27.
//

#include "device.h"
#include <iostream>

char defaultDeviceName[] = "COM3";
unsigned defaultBaudRate = 9600;

UartDevice::UartDevice():portName(defaultDeviceName),baudRate(defaultBaudRate){}

UartDevice::UartDevice(char* portName, unsigned baudRate):
		portName(portName),baudRate(baudRate){}

UartDevice::~UartDevice(){
	if(comHandle != INVALID_HANDLE_VALUE)
		CloseHandle(comHandle);
}

void UartDevice::setParams(char* portName, unsigned baudRate){
	if(comHandle != INVALID_HANDLE_VALUE)
		CloseHandle(comHandle);

	this->portName = portName;
	this->baudRate = baudRate;
}

int UartDevice::connect()
{

	comHandle = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	dcbParams.DCBlength = sizeof(dcbParams);
	if (comHandle == INVALID_HANDLE_VALUE)
	{
		printf("Cannot open %s.\n", portName);
		return 1;
	}
	if (!GetCommState(comHandle, &dcbParams))
	{
		printf("Cannot acquire COM status.\n");
		return 1;
	}
	dcbParams.BaudRate = baudRate;
	dcbParams.ByteSize = 8;
	dcbParams.StopBits = ONESTOPBIT;
	dcbParams.Parity = NOPARITY;
	if (!SetCommState(comHandle, &dcbParams))
	{
		printf("Cannot set COM status.\n");
		return 1;
	}
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (!SetCommTimeouts(comHandle, &timeouts))
	{
		printf("Cannot set timeout.\n");
		return 1;
	}
	return 0;
}

int UartDevice::write(char* buffer, unsigned bufferSize){
	return WriteFile(comHandle, buffer, bufferSize, (LPDWORD)&byteNumWritten, NULL);
}

int UartDevice::read(char* buffer, unsigned bufferSize){
	return ReadFile(this -> comHandle, buffer, bufferSize, (LPDWORD)&byteNumRead, NULL);
}

