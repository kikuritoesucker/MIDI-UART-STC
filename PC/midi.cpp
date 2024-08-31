#include "midi.h"
#include "device.h"
#include <iostream>
#include <cmath>
#include <bitset>
#include <stack>

extern char buffer[];
extern bool emit;
extern std::bitset<128> notePressed;
extern std::stack<unsigned char> noteStack;

void MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	if(wMsg == MIM_DATA){
		unsigned note = (dwParam1 & 0x0000ff00) >> 8;
		unsigned action = dwParam1 & 0x000000ff;
		float tick = 32e6 / (toFreq(note));
		auto pitch = (unsigned)ceil(tick);

		std :: cout <<
		"note=" << note <<
		", action=" << action <<
		", freq="  << (toFreq(note))<<
		", pitch=" << tick<<
		std::endl;

		char isNoteOn = static_cast<char>(action == eventNotePressed);
		if(isNoteOn) {
			notePressed.set(note, true);
			noteStack.push(note);
		}else{
			notePressed.set(note, false);
			while(!noteStack.empty() && !notePressed.test((unsigned)noteStack.top())){
				noteStack.pop();
			}
			if(!noteStack.empty()){

				note = noteStack.top();
				tick = 32e6 / (toFreq(note));
				pitch = (unsigned)ceil(tick);
				isNoteOn = 1;
			}
		}
		buffer[2] = static_cast<char>((pitch & 0x0000ff00) >> 8);
		buffer[3] = static_cast<char>((pitch & 0x000000ff));
		buffer[4] = isNoteOn;
		//uartDevice.write(buffer, 4);
		emit = true;
	}
}

float toFreq(const char hex) {
	const int semitones = (hex - A0Hex);
	float freq = A0Freq * pow(2, static_cast<float>(semitones) / 12);
	float coeff = 1/(1 + exp(-0.001*(freq - 3355)));
	return freq * (1 - coeff) + (pow(freq, 1.022) * 1.041 -801) * coeff;
}
