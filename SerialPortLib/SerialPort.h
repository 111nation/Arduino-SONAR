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
}

class SerialPort { 
	private:
	HANDLE hComPort;
	DCB dcbSettings = {0};

	public:

	SerialPort(std::string sPort);
	~SerialPort();
	void Settings(int Baud_Rate, int Bytes);
	
	// IO Operations
	void Write(std::string msg);
	std::string Read(int bytes_to_read);
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
			throw DISCONNECTED;
		} else {
			throw ACCESS_DENIED;
		}
	}

}


// GENERAL SETTINGS
void SerialPort::Settings(int Baud_Rate, int Bytes) {
	using namespace Error;
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
	
	DWORD bytesSent = 0; // Stores number of bytes sent: Ensures all data sent
	
	// Error writing message	
	if (!WriteFile(hComPort, msg.c_str(), msg.size() , &bytesSent, NULL)) {
		throw COMM_ERR;
	}

	FlushFileBuffers(hComPort);

}

std::string SerialPort::Read(int bytes_to_read) {
	using namespace Error;

	DWORD msg_bytes=0; 
	char msg[bytes_to_read + 1]; // Sets size for string in bytes, plus added space for null terminator

	if (!ReadFile(hComPort, msg, bytes_to_read, &msg_bytes, NULL)) {
		// Error reading message
		throw COMM_ERR;
	}

	msg[msg_bytes] = '\0'; // Closes string after data read;
	
	return  std::string(msg);

}

SerialPort::~SerialPort() { 
	using namespace Error;
	
	// Closes Port
	if (hComPort != INVALID_HANDLE_VALUE) {
		CloseHandle(hComPort);
	}
}






