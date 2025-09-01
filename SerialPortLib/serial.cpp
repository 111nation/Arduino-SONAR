#include <iostream>
#include <string>
#include <cctype>
#include "SerialPort.h"

bool run = true;

void getMessage (std::string* MESSAGE, SerialPort* PORT) {
	
	std::cout << std::endl;

	// Builds the message until the \n character to ensure message read properly per line
	while (1) {
		char cMsg = PORT->Read(1)[0]; // Converts to char
		
		// If message empty, exits loop
		if (cMsg == '\0') {
			*MESSAGE = "";
			break;
		}

		std::cout << cMsg << std::endl;
		
		if (cMsg == '\n') {
			// Signal End of message, close loop
			std::cout<<std::endl;
			break;
		} else {
			// Gather character
			*MESSAGE += std::string(1,cMsg);
		}
	}
}

void program () {

	try {
		// ===== SETUP PORT ========================
	       	SerialPort ARDUINO("COM3");

		ARDUINO.Settings(9600, 8); 

		// ===== USER COMMAND ======================
		std::string input;
		getline(std::cin, input);

		// ===== SENDS COMMAND TO ARDUINO ==========
		if (input == "on") {
			ARDUINO.Write("1");
			std::cout << "\b\rBulb STATUS: ON\n";
		}
		else if (input == "off"){
			ARDUINO.Write("0");
			std::cout << "\b\rBulb STATUS: OFF\n";
		} else if (input == "quit") {
			run = false;		// QUITS PROGRAM			
		} else {
			std::cout << "\b\rInvalid Command\n";
		}

		std::string msg; 
		getMessage(&msg, &ARDUINO);
		std::cout << msg << "\n";

	} catch (int error) { 
		// ==================================================
		// 		     ERROR HANDLING
		// ==================================================

        using namespace Error;
		switch (error) {
			case DISCONNECTED: 
				std::cout << "DISCONNECTED\n";
				break;

			case ACCESS_DENIED:
				std::cout << "PORT IN USE\n";
				break;
	
			case CONFIG_ERR:
				std::cout << "COULD NOT CONFIGURE PORT\n";
				break;

			case COMM_ERR:
				std::cout << "COULD NOT COMMUNICATE\n";
				break;
		}

		run = false;
	}
}

int main () {
	// Runs program until user quits
	while (run == true) {
		program();
	}

	return 0;
}
