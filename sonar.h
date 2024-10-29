#pragma once

#include "SerialPortLib/SerialPort.h"

class SONAR {
	public:
	int status = 0;
	int rate = 9600;

	std::string com;

	~SONAR();

	void Init();

	private:
	SerialPort *ARDUINO = NULL;
	const int byte_size = 8; 
};
