#include "main.h"

bool hover_minimize = false;
bool hover_exit = false;
int click_minimize = false;
int click_exit = false;

const std::wstring MAIN_CLASS = L"MAIN_CLASS";

#define FPS 1
#define APPLICATION_TIMER 1

namespace sonar {
	double deg = -45;
	bool clockwise = true;
	const int INTERVAL = 1;
}	

LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hApp, HINSTANCE hPrev, PSTR arg, int window_mode) {
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hApp;
	wc.lpszClassName = MAIN_CLASS.c_str();
	RegisterClass(&wc);

	const int windowx = (int) (1920 - WINDOW_WIDTH)/2;
	const int windowy = (int) (1080 - WINDOW_HEIGHT - TITLE_BAR)/2;
	HWND hDashboard = CreateWindowEx(
				WS_EX_LAYERED, MAIN_CLASS.c_str(),
				L"SONAR",
				WS_POPUP,
				windowx, windowy,
				WINDOW_WIDTH, WINDOW_HEIGHT + TITLE_BAR, // Size
				NULL, NULL,
				hApp, NULL);

	if (hDashboard == NULL) {
		MessageBox(NULL, L"SONAR failed to open", L"Error", MB_ICONERROR);
		return 0;
	}

	// Timer used to remind application to update display
	// Timer used to connect to the arduino board
	SetTimer(hDashboard, APPLICATION_TIMER, FPS, NULL);

	// Set Transparency of window
	//SetLayeredWindowAttributes(hDashboard, DEF_TRANSPARENT, 0, LWA_COLORKEY);
	ShowWindow(hDashboard, window_mode);  

	//===============================================
	// 	MESSAGE LOOP
	//===============================================
	MSG event = {};
	while(GetMessage(&event, NULL, 0, 0) > 0) {
		TranslateMessage(&event);
		DispatchMessage(&event);
	}	

	KillTimer(hDashboard, APPLICATION_TIMER);

	return 0;		
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam) {
	switch (event) {
		case WM_CREATE: {
			Paint paint;
			paint.hWnd = hWnd;
			paint.Transparency();

			SONAR.com = "COM3";
			SONAR.Init();

			break;
		}

		case WM_ERASEBKGND: {
			return TRUE;
			break;
		}

		case WM_PAINT: {
			//=====PAINTING DATA========
			Paint paint;
			PAINTSTRUCT ps;
			paint.area = BeginPaint(hWnd, &ps);

			//=====TRANSPARENCY=========
			paint.x = 0;
			paint.y = 0;
			paint.xend = WINDOW_WIDTH;
			paint.yend = WINDOW_HEIGHT + TITLE_BAR; 
			paint.color = DEF_TRANSPARENT;
			paint.border.color = DEF_TRANSPARENT;
			paint.border.width = 1;

			paint.Rectangle();
			
			//=====BACKGROUND===========
			paint.RECTtoPos(ps.rcPaint);
			paint.color = BACKGROUND_COLOR;

			//====WINDOW BORDERS========
			paint.hWnd = hWnd;
			paint.border.width = 1;
			paint.border.style = PS_SOLID;
			paint.border.color = border_color; 
			
			paint.RoundRect(window_corner);

			//=========TITLE BAR========
			paint.Reset();
			paint.x = 0;
			paint.y = 0;
			paint.xend = WINDOW_WIDTH;
			paint.yend = TITLE_BAR-1;
			paint.color = 0;
			paint.border.width = 1;
			paint.border.color = border_color;

			paint.RoundRect(window_corner);
			
			paint.border.width = 0;
			paint.y = (int) (window_corner/2 - 1);
			paint.yend = TITLE_BAR;
			paint.x = 1;
			paint.xend = WINDOW_WIDTH;

			paint.Rectangle();

			// Title Bar Buttons
			Paint_Minimize(hWnd, hover_minimize);
			Paint_Exit(hWnd, hover_exit);

			
			//TITLE TEXT
			paint.font.name = "Montserrat";
			paint.font.size = 17;
			paint.font.weight = 900;
			paint.x = 12;
			paint.y = 2;
			paint.color = ACCENT_0;

			paint.Text("SONAR");

			//=======SONAR BACKGROUND=======
			paint.Reset();
			paint.x = sonar_bg_margin;
			paint.y = TITLE_BAR-1;
			paint.xend = WINDOW_WIDTH;
			paint.yend = WINDOW_HEIGHT + TITLE_BAR;
			paint.color = MID_COLOR;

			paint.RoundRect(window_corner);

			paint.y = WINDOW_HEIGHT - window_corner;
			paint.xend = (int) WINDOW_WIDTH / 2;
			paint.Rectangle();

			paint.y = TITLE_BAR-1;
			paint.x = (int) WINDOW_WIDTH / 2;
			paint.yend = (int) (WINDOW_HEIGHT + TITLE_BAR) / 2;
			paint.xend = WINDOW_WIDTH;
			paint.Rectangle();


			//========STATUS===========
			// Initialize SONAR display
			Paint_Status(hWnd, SONAR.status);			
			
			
			EndPaint(hWnd, &ps);
			break;
		}
	
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
			break;
		}

		case WM_SETCURSOR: {
			WORD cursPos = LOWORD(lParam);
			
			// Set cursor to pointer
			if (cursPos == HTCLIENT) {
				HCURSOR hcursor = LoadCursor(NULL, IDC_ARROW);
				SetCursor(hcursor);
				return TRUE;	
			}

			break;
		}


		//=======================
		// EVENT HANDLER
		//=======================
		case WM_LBUTTONDOWN: {
			// Get mouse/ curser position
		  	POINT mouse;
			GetCursorPos(&mouse);
			ScreenToClient(hWnd, &mouse);

			if (mouse.y >= 0 && mouse.y <= TITLE_BAR) {
				if (mouse.x >= 0 && mouse.x <= WINDOW_WIDTH - button_width*2) {
					SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
				}
				
				// Title buttons
				if (hover_minimize) {
					click_minimize = true;
				} else if (hover_exit){
					click_exit = true;
				}
			}

			break;

		}

		case WM_LBUTTONUP: {
		  	POINT mouse;
			GetCursorPos(&mouse);
			ScreenToClient(hWnd, &mouse);
			
			// Checks if user confirms click on title bar buttons
			if (mouse.y >= 0 && mouse.y <= TITLE_BAR) {
				if (hover_minimize) {
					// Minimize
					ShowWindow(hWnd, SW_MINIMIZE); 
				} else if (hover_exit){
					// Exit
					SendMessage(hWnd, WM_DESTROY, 0, 0);
				}
			}
			
			break;
		}

		case WM_MOUSEMOVE: {
			POINT mouse;
			GetCursorPos(&mouse);
			int in_focus = ScreenToClient(hWnd, &mouse);

			// Checks if curser in title bar
			bool in_title = (mouse.y >= 0 && mouse.y <= TITLE_BAR) && 
					(mouse.x >= 0 && mouse.x <= WINDOW_WIDTH);

			if (in_title && in_focus > 0) {
				// Determines which button is hovered over
				if (mouse.x >= minimize_x && mouse.x < minimize_xend) {
					// Minimize
					SetCapture(hWnd);
					hover_minimize = true;
					hover_exit = false;
					
				} else if (mouse.x >= exit_x && mouse.x <= exit_xend) {
					// Exit 
					SetCapture(hWnd);
					hover_minimize = false;
					hover_exit = true;

				} else {
					ReleaseCapture();
					hover_minimize = false;
					hover_exit = false;
				}
			} else {
					ReleaseCapture();
					hover_minimize = false;
					hover_exit = false;
			}

			Paint_Minimize(hWnd, hover_minimize);
			Paint_Exit(hWnd, hover_exit);


			break;
		}

		//==========================
		// REGULAR INTERVALS
		//==========================
		case WM_TIMER: {
			
			switch (wParam) {

				// DISPLAY TIMER
				case APPLICATION_TIMER: {
					// CONNECTS TO ARDUINO
					Paint_Status(hWnd, SONAR.status);			
					if (SONAR.status != STATUS::DISCONNECTED) {
						SONAR.com = "COM3";
						SONAR.Init();
						SonarDisplay(hWnd, -135);
						break;
					}

					// UPDATES COORDINATES OF ARDUINO
					if (SONAR.Update()) break;
					
					//if (SONAR.PostConfirm()) break;

					// MOVES THE SONAR DISPLAY TO AREA
					SonarDisplay(hWnd, SONAR.deg);					
					
					return 0;

					break;
				}

			}

			
			break;
		}
	}
	return DefWindowProc(hWnd, event, wParam, lParam);
}

// SONAR CLASS
bool SONAR::Init() {
	if (ARDUINO) {
		delete ARDUINO;
	}

	try {
		ARDUINO = new SerialPort(com);
		status = STATUS::OK;
	} catch (int err) {
		// Converts errors into status codes
		switch (err) {
			case Error::DISCONNECTED:
				status = STATUS::DISCONNECTED;
				break;
			default:
				status = STATUS::WARNING;
				break;
		}

		delete ARDUINO; 

		return true;	
	}

	return false;
}

bool SONAR::Update() {
	try {
		std::string data = ARDUINO->Read(BYTES);
		// Attempts to convert message
		if (ConvertMessage(data) != 0) {
			status = STATUS::WARNING;
		
			// Let Arduino know to halt due to error
			PostError();
			return true;
		} else {
			//MessageBoxA(NULL, std::to_string(deg).c_str(), "DEG", MB_OK);
		}
	} catch (int err) {
		PostError();
		GetErr(err);
		return true;
	}
	
	PostConfirm();

	return false;
}

bool SONAR::PostError() {
	return SendMsg("ERR");
}

bool SONAR::PostConfirm() {
	//MessageBoxA(NULL, std::to_string(ARDUINO->size()).c_str(), "SIZE", MB_OK);
	return SendMsg("OK");
}

bool SONAR::SendMsg(std::string msg) {
	try {
		std::stringstream ss;
		ss << cMSG << msg << cEND;
	//	MessageBoxA(NULL, ss.str().c_str(), "DEG", MB_OK);
		ARDUINO->Write(ss.str());
	} catch (int err) {
		GetErr(err);	
		return true;
	}
	return false;
}

SONAR::~SONAR () {
	if (ARDUINO != NULL) {
		delete ARDUINO;
	}
}

//====================
// UTILITIES
//====================
int SONAR::ConvertMessage(std::string msg) {
	const int old_deg = deg;
	const int old_prox = proximity;

	std::string sDeg = "";
	std::string sProx = "";

	// Breaks string into Degree and Proximity sections
	bool parse = false;
	bool parsedDeg = false;
	bool end = false;

	for (unsigned int i=0; i < msg.length(); i++) {
		// If detects special character
		// tells program what to do
		switch (msg[i]) {
			case cBEGIN:
				parse = true;
				continue;
			case cDELIM:
				if (parse && !end) {
					parsedDeg = true;
				}
				continue;
			case cEND:
				if (parse && parsedDeg) {
					end = true;
				}
				break;
		}

		// Extracts information
		if (parse && !end) {
			if (parsedDeg == false) {
				// PARSING DEGREE
				sDeg += msg[i];
			} else {
				sProx += msg[i];
			}
		}
	}
	
	try {
		if (parse == false || sDeg == "" || sProx == "" || end == false) {
			throw "Data recieved was incomplete";	
		}
			
		// NOTE: please add another buffer variable logic to store incomplete messages there so that
		// 	i can wait for next piece of buffer and recontinue reading so that program is optimized

		deg = std::stoi(sDeg);
		deg -= 90; // Match the display output degree system
		proximity = std::stoi(sProx);
	// Reroll to previous versions if failed
	} catch (...) {
		deg = old_deg;
		proximity = old_prox;
		return 1;
	}

	return 0;
	
}

void SONAR::GetErr(int err) {
	// Converts errors into status codes
	switch (err) {
		case Error::DISCONNECTED:
			status = STATUS::DISCONNECTED;
			break;
		default:
			status = STATUS::WARNING;
			break;
	}
}
