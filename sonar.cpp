#include "sonar.h"

#include <sstream>

bool SONAR::Init() {
	// Tries to initialize and open the serial port 
	// for reading
	ClearHandle();
	
	deg = 0;
	prox = 0;

	hPort = CreateFileA(
			port.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
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

	if (isOverflow()) {
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
	dcb.BaudRate = CBR_115200;
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
	COMSTAT status; 

	if (isOverflow(&status)) {
		return true;
	}	

	// Only start reading if minimum 5 bytes #0:0\n
	if (status.cbInQue >= 5) {
		// Start reading data from port
		char in[MAX_READ + 1];
		DWORD bytes;

		// Data read in
		if (!ReadFile(hPort, &in, MAX_READ, &bytes, NULL)) {
			GetStatus();
			return true;
		}		

		if (bytes >= 5) {
			// Build data
			in[bytes] = '\0';
			msg = std::string(in);

			Parse();
		} else {
			return true; // Full message not gathered
		}
	}

	return false;
}

bool SONAR::Write(int deg, int prox) {
	// Puts message in correct format
	std::string msg = cMSG + std::to_string(deg) + cDELIM + std::to_string(prox) + cEND;

	if (isOverflow()) { // Checks and clears port if an overflow
		return true;
	}

	const char *buf = msg.c_str();
	DWORD bytes;
	
	if (!WriteFile(hPort, buf, msg.length(), &bytes, NULL)) {
		GetStatus();
		return true;
	}

	// Determines if whole message sent
	if (bytes == msg.length()) {
		return false; // Successful
	} else { 
		GetStatus();
		return true; // Incomplete operation
	}
}

bool SONAR::isOverflow() {
	// isOverflow without parameters
	COMSTAT status;
	return isOverflow(&status);
}

bool SONAR::isOverflow(LPCOMSTAT status) {
	// Checks if device is overflowing
	// Clears device in an event of overflow
	DWORD errors;
	
	// Checks for overflow
	if (ClearCommError(hPort, &errors, status)) {
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

	return false;
}

void SONAR::ClearHandle () {
	if (hPort) {
		CloseHandle(hPort);
	}
}

// AUXILLARY METHODS
bool SONAR::Parse() {
	// Checks if message complete before proceeding
	
	bool begin = false;	// Tracks format of string
	bool end = false;
	bool delim = false;
	std::string temp = msg;

	msg = "";
	for (unsigned int i = 0; i < temp.length(); i++) {
		// Truncates msg variable into a shorter and easily parseable string
		switch (temp[i]) {
			case cBEGIN:
				begin = true;
				break;
			case cEND:	
				// Only indicate end if we started message
				if (begin) {
					end = true;
				}
				break;

			default:
				if (begin && !end) {
					if (temp[i] == cDELIM) {
						delim = true;
					}
					
					msg += temp[i];
				}
				break;
		}	
		
		if (end) break; // Breaks out of loop when done
	}

	// Checks if string in correct format (aka has a delimeter and has begin and end)
	if (begin && end && delim && !msg.empty()) {
		// Begin actual parsing	
		std::string sDeg = "";	// String version of proximity and degree
		std::string sProx = "";
		
		const size_t delim_pos = msg.find(cDELIM);
		if (delim_pos == std::string::npos) {
			return true; // Delimeter not found
		}


		sDeg = msg.substr(0, delim_pos);
		sProx = msg.substr(delim_pos+1, msg.length()-delim_pos-1);
	
		// Attempts parsing degree and proximity
		int oldDeg = deg;
		int oldProx = prox;

		std::stringstream ssDeg(sDeg);
		std::stringstream ssProx(sProx);

		if (!(ssDeg >> deg) || !(ssProx >> prox)) {
			deg = oldDeg;
			prox = oldProx;
			return true;
		}
	} else {
		return true;
	}

	return false;	
}


SONAR::~SONAR () {
	ClearHandle();
}
