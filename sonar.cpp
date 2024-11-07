#include "sonar.h"

bool SONAR::Init() {
	// Tries to initialize and open the serial port 
	// for reading
	ClearHandle();
	
	hPort = CreateFileA(
			port.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_OFFLINE,
			NULL);

	if (GetStatus()) {
		return true;
	}


	if (Configure()) {
		return true;
	}

	// Successful
	status = STATUS::OK;

	return false;

}

bool SONAR::Configure() {
	// Configures serial port with related settings
	DCB dcb = {0};
	dcb.DCBlength = sizeof(dcb);

	if (!GetCommState(hPort, &dcb)) {
		GetStatus();
		return true;
	}	

	// SERIAL - SETTINGS
	dcb.BaudRate = CBR_9600;
	dcb.fParity = FALSE;
	dcb.fNull = TRUE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;

	dcb.DCBlength = sizeof(dcb);
	if (!SetCommState(hPort, &dcb)) {
		GetStatus();
		return true;
	}

	return false;
}

// UTILITES
bool SONAR::GetStatus() {
	DWORD error;
	COMSTAT status;
	
	if (hPort == INVALID_HANDLE_VALUE) {
		this->status = STATUS::DISCONNECTED;		
	} else {
		if (ClearCommError(hPort, &error, &status)) {
			// Checks error status of port	
			if (error == 0) {
				this->status = STATUS::OK;
			} else if (error == CE_BREAK) {
				this->status = STATUS::DISCONNECTED;
			} else {
				this->status = STATUS::WARNING;
			}
		} else {
			
			if (GetLastError() == ERROR_FILE_NOT_FOUND) {
				this->status = STATUS::DISCONNECTED;
			} else {
				this->status = STATUS::WARNING;
			}
			
			/*switch (GetLastError()) {
				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
				case ERROR_INVALID_HANDLE:
				case ERROR_SERIAL_NO_DEVICE:
				case ERROR_DEV_NOT_EXIST:
					this->status = STATUS::DISCONNECTED;
					break;
					
				default:
					this->status = STATUS::WARNING;
					break;
			}*/
		}
	}


	if (this->status == STATUS::OK) {
		return false; // No errors
	} else {
		return true; // Errors
	}

}

// I/O 
bool SONAR::Read() {
	// Checks if device is overflowing
	DWORD errors;
	COMSTAT status;

	// Checks for overflow
	if (!ClearCommError(hPort, &errors, &status)) {
		if (errors == CE_OVERRUN || errors == CE_RXOVER) {		
			if (!PurgeComm(hPort, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR)) {
				GetStatus();
				return true;
			}
		}
	} else {
		GetStatus();
		return true;
	}
	
	// Only start reading is minimum 5 bytes #0:0\n
	if (status.cbInQue >= 5) {
		// Start reading data from port

	}

	return false;
}

void SONAR::ClearHandle () {
	if (hPort) {
		CloseHandle(hPort);
	}
}

SONAR::~SONAR () {
	ClearHandle();
}
