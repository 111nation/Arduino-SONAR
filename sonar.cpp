#include "sonar.h"

void SONAR::Init() {
	if (ARDUINO != NULL) {
		delete ARDUINO;
	}

	try {
		ARDUINO = new SerialPort(com);
	} catch (int err) {
		delete ARDUINO; 	
		status = err;
		
		if (err == DISCONNECTED) {
			MessageBox(NULL, "Disconnected", "ERROR", MB_ICONERROR);
		}
	}

	status = 0;
}

SONAR::~SONAR () {
	if (ARDUINO != NULL) {
		delete ARDUINO;
	}
}
