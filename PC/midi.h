//
// Created by 26808 on 24-8-27.
//

#ifndef UART_MIDI_H
#define UART_MIDI_H
#include "windows.h"
#include "mmeapi.h"

const char A0Hex  = 0x21;
const unsigned A0Freq = 440;

const unsigned char eventNotePressed = 144;
const unsigned char eventNoteReleased = 128;

float toFreq(const char hex);
void MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

#endif //UART_MIDI_H
