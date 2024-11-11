#pragma once

#include <string>
#include "status.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//=======================SONAR HANDLING===================
// functions with bool data type return true in an event of an error
// false if completed successfully
class SONAR {
	private:
	enum {
		MAX_READ = 32 // MAXIMUM OF 32 bytes to read NB: If sonar updates to slow at higher deg
			      // increase the maximum bytes to read
	};

	// MESSAGING PROTOCOL
	   // Arduino -> Program
	   static const char cBEGIN = '#';
	   static const char cDELIM = ':';
	    
	   // Program -> Arduino
	   static const char cMSG = '!';
	   static const char cEND = '\n';		

	HANDLE hPort;

	bool Parse();
	bool isOverflow(LPCOMSTAT status);
	bool isOverflow();

	public:
	std::string port;	
	int status = STATUS::DISCONNECTED;
	std::string msg = ""; // Message from arduino

	// SONAR details
	int deg = 0;
	int prox = 0; // 2 - 300 cm

	bool Init();
	bool Configure();
	void ClearHandle();

	// I/O Operations
	bool Read();
	bool Write(int deg, int prox);

	// UTILITES
	bool GetStatus();


	~SONAR();
};

