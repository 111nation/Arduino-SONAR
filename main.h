#pragma once

#include "display.h"
#include "SerialPortLib/SerialPort.h"

//=======================SONAR HANDLING===================
class SONAR {
	public:
	int status = STATUS::DISCONNECTED;
	int rate = 9600;
	int hello = 0;

	std::string com;
	
	~SONAR();

	void Init();

	private:
	SerialPort *ARDUINO = NULL;
	const int byte_size = 8; 
};

SONAR SONAR;

