#pragma once
//*****************************************////*****************************************//
//------- midi parser v0 nik
// using rtMidi , a modified version of qmidiin.cpp by Gary Scavone, 2003-2004.
//*****************************************////*****************************************//

#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "RtMidi.h"

class noteData
{
public:

	bool gate = false;
	bool latch = false;
	int pitch = 0;
	int vel = 127;
	float gateLen = 100;
	int aftertouch = 0;
	// -5  Volts = midi 0   = 2 octaves below the lowest note on a standard 88-key piano
	// 5.5 volts = midi 127 = 1.5 octaves above the highest piano note

	noteData() {};

	void print()
	{
		if (gate == true)
		{
			std::cout << "      Note = ON" <<
				"\n     Pitch = " << pitch <<
				"\n       Vel = " << vel <<
				"\nGateLength = " << gateLen << std::endl;
		}
		else {
			std::cout << "      Note = OFF" <<
				"\nGateLength = " << gateLen << std::endl;
		}
	}
};

class channelData
{
public:

	noteData note;
	int   noteBuffer;
	bool  CCtype = false;
	float CC = 0;
	int   CCid = 0;
	int   channel = 0;
	int   pitchbend = 0;
	int   channelPressure = 0;
	int   delta = 0;
	int   patchID = 0;

	channelData() {};

	void SetNote()
	{
		delta = note.pitch - noteBuffer;
		noteBuffer = note.pitch;
	}

	float deltaBi()
	{
		return (delta == 0) ? 0 : (float)delta / 256.0;
	}
	float noteNormal()
	{
		return (note.pitch == 0) ? 0 : (float)note.pitch / 128;
	}
	void print()
	{
		std::cout << "   Channel : " << channel << std::endl;

		if (CCtype)
		{
			std::cout << "\n ControlChange ID " << CCid <<
				"\n        CC value: " << CC << std::endl;
		}
		else {
			note.print();
			std::cout << " NoteDelta = " << deltaBi() << std::endl;
		}
	}
};

class MidiBaron
{
public:
	MidiBaron() {}
	~MidiBaron() { delete midiin; }

	RtMidiIn* midiin = 0;
	std::vector<unsigned char> message;
	int nBytes, i;
	double stamp;
	unsigned int port = 0;
	unsigned int nPorts = 0;
	channelData status;


	void init(int a, char* argv[])
	{
		// Minimal command-line check.
		if (a > 2) usage();
		std::cout << "init" << std::endl;
		// RtMidiIn constructor
		try {
			midiin = new RtMidiIn();
			std::cout << "made new one " << std::endl;
		}
		catch (RtMidiError& error) {

			error.printMessage();
			exit(EXIT_FAILURE);
		}

		// Check available ports vs. specified.
		port = 0;
		nPorts = midiin->getPortCount();
		if (a == 2) port = (unsigned int)atoi(argv[1]);
		if (port >= nPorts) {
			delete midiin;
			std::cout << "deleted midiin" << std::endl;
			std::cout << "Invalid port specifier!\n";
			usage();
		}
		try {
			midiin->openPort(port);
			std::cout << "opened port " << std::endl;
		}
		catch (RtMidiError& error) {
			error.printMessage();
			delete midiin;
			std::cout << "deleted midiin" << std::endl;
		}


	}

	void usage(void) {
		// Error function in case of incorrect command-line
		// argument specifications.
		std::cout << "\nusage: qmidiin <port>\n";
		std::cout << "    where port = the device to use (first / default = 0).\n\n";
		exit(0);
	}

	channelData probeInput() // control timing around this
	{
		stamp = midiin->getMessage(&message);
		nBytes = message.size();

		for (i = 0; i < nBytes; i++)// it seems to crash if it is not run inside a loop of nBytes length.. I don't understand rtMidi well.
		{
			if (i == 0)
			{
				int mDat = (int)message[0];
				std::cout << "\nincoming message: " << mDat << std::endl;
				int channel;

				if (mDat >= 0x80 && mDat <= (0xE0 + 15))       //(0-15 channels) // check message is a voice message (has a channel) https://stackoverflow.com/questions/15993856/getting-midi-channel-from-midistatus
				{

					channel = (mDat & 0x0F);
					status.channel = channel;
					//std::cout << "channel: " << channel << std::endl;

					if (mDat == 0x90 + channel) // note ON
					{
						status.note.gate = true;
						status.note.latch = true;
						status.note.pitch = (int)message[1];
						status.note.vel = (int)message[2];
						status.SetNote();
						//    std::cout << "note ON, pitch: " << status.note.pitch << std::endl;
					}

					if (mDat == 0x80 + channel) // note OFF
					{
						status.note.gate = false;
						status.note.gateLen = stamp;
						status.note.latch = false;
						//    std::cout << "note OFF, lasted: " << status.note.gateLen << std::endl;
					}

					if (mDat == 0xA0 + channel)     //Aftertouch
					{
						status.note.pitch = (int)message[1];
						status.note.aftertouch = (int)message[2];
						//    std::cout << "Aftertouch: " << status.note.aftertouch << std::endl;
						//    std::cout << "   on note: " << status.note.pitch      << std::endl;
					}

					if (mDat == 0xB0 + channel)     //Continuous controller change
					{
						status.CCtype = true;
						status.CCid = (int)message[1]; // byte 1 = cc id
						status.CC = (int)message[2]; // byte 2 = cc value
						//    std::cout << "   CC id: " << status.CCid << std::endl;
						//    std::cout << "CC value: " << status.CC   << std::endl;
					}
					else { status.CCtype = false; }

					if (mDat == 0xC0 + channel)     //Patch change
						status.patchID = (int)message[1];
					//std::cout << "Patch ID: " << status.patchID << std::endl;

					if (mDat == 0xD0 + channel)     //Channel Pressure
					{
						status.channelPressure = (int)message[1];
						//    std::cout << "Channel Pressure on CHANNEL: " << status.channelPressure << std::endl;
					}
					if (mDat == (0xE0 + channel))     //Pitch bend
					{
						status.pitchbend = ((int)message[2] * 128) + (int)message[1];
						//    std::cout << "Pitch bend: " << status.pitchbend << std::endl;
					}
				}
				status.print();
			}

		}

		return status;
	}
};