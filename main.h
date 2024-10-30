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


	std::string com;
	
	~SONAR();

	bool Init();
	bool Update();
	bool PostError();
	bool PostConfirm();

	private:
	const int BYTES = 8;
	SerialPort *ARDUINO = NULL;
	const int byte_size = 8; 
	
	int ConvertMessage(std::string msg);
	void GetErr(int err);
};

SONAR SONAR;

