#pragma once

#include "display.h"
#include "SerialPortLib/SerialPort.h"

//=======================SONAR HANDLING===================
class SONAR {
	public:
	int status = STATUS::DISCONNECTED;
	int rate = 9600;
	int hello = 0;
	int deg = -90;
	int proximity = 0;


	std::string com;
	
	~SONAR();

	bool Init();
	bool Update();

	// WRITING TO ARDUINO
	bool SendMsg(std::string msg);
	bool PostError();
	bool PostConfirm();

	private:
	const int BYTES = 12; // Gets maximim size of arduino output
	SerialPort *ARDUINO = NULL;
	const int byte_size = 8; 
	
	// Arduino -> Program
		static const char cBEGIN = '#';
		static const char cDELIM = ':';

	// Program -> Arduino
		static const char cMSG = '!';

		static const char cEND = '\n';
	
	int ConvertMessage(std::string msg);
	void GetErr(int err);
};

SONAR SONAR;

