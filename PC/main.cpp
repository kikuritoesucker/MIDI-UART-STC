#include <iostream>
#include <bitset>
#include <stack>
#include "windows.h"
#include "mmeapi.h"

#include "device.h"
#include "midi.h"

const char* deviceName = "COM3";
const unsigned int baudRate = 9600;

unsigned char buffer[8] = {0xFF,0xAA,};
bool emit = false;

std::bitset<128> notePressed;
std::stack<unsigned char> noteStack;

int main()
{
	HMIDIIN midiHandle;
	uartDevice.setParams(
			(char*)deviceName,
			baudRate);
	std::cout << "Connecting to " << deviceName <<" @ "<<baudRate<<"MHz"<<std::endl;
	if(uartDevice.connect() != 0) {
		std::cerr << "Failed to establish connection with given port name and Baud rate." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Connection established."<<std::endl;
	MMRESULT result = midiInOpen(&midiHandle, 0, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION);
	if (result != MMSYSERR_NOERROR)
	{
		std::cerr << "Failed to open MIDI input device."<<std::endl;
		exit(EXIT_FAILURE);
	}
	midiInStart(midiHandle);



	while(true){
		if(emit)
		{
			uartDevice.write((char*)buffer, sizeof(buffer) / sizeof(char));
			emit = false;
		}
		Sleep(10);
	}
	midiInClose(midiHandle);
	return 0;
}