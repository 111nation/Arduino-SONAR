#pragma once

#include <string>
#include "status.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//=======================SONAR HANDLING===================
class SONAR {
	private:
	enum {
		MAX_READ = 12 // MAXIMUM OF 12 bytes to read
	};

	// MESSAGING PROTOCOL
	   // Arduino -> Program
	   static const char cBEGIN = '#';
	   static const char cDELIM = ':';
	    
	   // Program -> Arduino
	   static const char cMSG = '!';
	   static const char cEND = '\n';		

	HANDLE hPort;
	bool GetStatus();

	std::string msg = ""; // Message from arduino

	public:
	std::string port;	
	int status = STATUS::DISCONNECTED;

	// SONAR details
	int deg = -90;
	int prox = 0;

	bool Init();
	bool Configure();
	void ClearHandle();

	// I/O Operations
	bool Read();

	~SONAR();
};

