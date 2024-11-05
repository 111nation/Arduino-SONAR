#pragma once // Prevents circular reference
	   
#define WIN32_LEAN_AND_MEAN // Excludes unnecissary API features
#include <windows.h>
#include <string>

// I_O STATE
#define READ GENERIC_READ
#define WRITE GENERIC_WRITE

// ==========================================================
// 		Easy Access to Serial Port
// ==========================================================

// ERRORS
namespace Error {
	const int DISCONNECTED = 1;
	const int ACCESS_DENIED = 2;
	const int CONFIG_ERR = 3;
	const int COMM_ERR = 4;
	const int UNINITIALIZED = 5;
}

class SerialPort { 
	private:
	HANDLE hComPort;
	DCB dcbSettings = {0};
	void CheckOverflow();
	bool init=false; // Checks if the serial port initialized
			 // Prevents accessing port if not initialized

	public:
	SerialPort(std::string sPort);
	~SerialPort();
	void Settings(int Baud_Rate, int Bytes);
	
	// IO Operations
	void Write(std::string msg);
	std::string Read(int bytes_to_read);
	int size();
};

// ==========================================================
// 		Defenitions
// ==========================================================

SerialPort::SerialPort(std::string sPort) {
	using namespace Error;
	// Opens Port for read/ writing

	hComPort = CreateFileA(sPort.c_str(), READ | WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Potential Errors when opening Port
	if (hComPort == INVALID_HANDLE_VALUE){
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			init = false;
			throw DISCONNECTED;
		} else {
			init = false;
			throw ACCESS_DENIED;
		}
	}
	
	init = true;

	// Set timeouts to prevent indefinite blocking
	/*COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 1;          
	timeouts.ReadTotalTimeoutConstant = 10;     
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 10;    
	timeouts.WriteTotalTimeoutMultiplier = 1;

	if (!SetCommTimeouts(hComPort, &timeouts)) {
		CloseHandle(hComPort);
		throw ACCESS_DENIED; // Handle failed timeout setting
	}*/	

}


// GENERAL SETTINGS
void SerialPort::Settings(int Baud_Rate, int Bytes) {
	using namespace Error;
	if (!init) throw UNINITIALIZED ;

	dcbSettings = {0};
	dcbSettings.DCBlength = sizeof(dcbSettings);
		
	// Attempt to recieve existing settings
	if (!GetCommState(hComPort, &dcbSettings)) {
		throw CONFIG_ERR;
	}

	dcbSettings.BaudRate = Baud_Rate;
	dcbSettings.ByteSize = Bytes;

	dcbSettings.StopBits = ONESTOPBIT;
	dcbSettings.Parity = NOPARITY;

	// Sets the configuration on Port
	if (!SetCommState(hComPort, &dcbSettings)) {
		throw CONFIG_ERR;
	}
}

void SerialPort::Write(std::string msg) {
	using namespace Error;
	if (!init) throw UNINITIALIZED ;
	
	CheckOverflow();

	DWORD bytesSent = 0; // Stores number of bytes sent: Ensures all data sent
	
	// Error writing message	
	if (!WriteFile(hComPort, msg.c_str(), msg.size() , &bytesSent, NULL)) {
		throw COMM_ERR;
	}

	FlushFileBuffers(hComPort);

}

std::string SerialPort::Read(int bytes_to_read) {

	if (init != true) {
 		MessageBoxA(NULL, "TRUE", "TRUE", MB_OK);	
	} else {
 		MessageBoxA(NULL, "FALSE", "FALSE", MB_OKCANCEL);	
	}

	using namespace Error;
	if (!init) throw UNINITIALIZED;

	CheckOverflow();

	DWORD msg_bytes=0; 
	char msg[bytes_to_read + 1]; // Sets size for string in bytes, plus added space for null terminator

	if (!ReadFile(hComPort, msg, bytes_to_read, &msg_bytes, NULL)) {
		// Error reading message
		throw COMM_ERR;
	}

	msg[msg_bytes] = '\0'; // Closes string after data read;
	
	return  std::string(msg);

}

int SerialPort::size() {
	using namespace Error;
	if (!init) throw UNINITIALIZED ;
	
	COMSTAT comStat = {0};
	DWORD errors;

	if (ClearCommError(hComPort, &errors, &comStat)) {
		return comStat.cbInQue + comStat.cbOutQue;
	} else {
		return 0;
	}
}

void SerialPort::CheckOverflow() {
	using namespace Error;
	if (!init) throw UNINITIALIZED ;

	using namespace Error;
	// Prevents buffer overflow
	
	DWORD err;
	
	if (!ClearCommError(hComPort, &err, NULL)) {
		throw COMM_ERR;
	}

	// Clears buffer if overflow
	if (err == CE_OVERRUN || err == CE_RXOVER) {
		if (!PurgeComm(hComPort, PURGE_RXCLEAR)) {
			throw COMM_ERR;
		}
	}
	
}

SerialPort::~SerialPort() { 
	using namespace Error;
	
	// Closes Port
	if (hComPort != INVALID_HANDLE_VALUE) {
		CloseHandle(hComPort);
	}
}





