#include "dashboard.h"

const std::wstring MAIN_CLASS = L"MAIN_CLASS";

LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hApp, HINSTANCE hPrev, PSTR arg, int window_mode) {
	
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hApp;
	wc.lpszClassName = MAIN_CLASS.c_str();
	RegisterClass(&wc);

	HWND hDashboard = CreateWindowEx(
				0, MAIN_CLASS.c_str(),
				L"SONAR",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, // Position
				WINDOW_WIDTH, WINDOW_HEIGHT, // Size
				NULL, NULL,
				hApp, NULL);

	if (hDashboard == NULL) {
		MessageBox(NULL, L"SONAR failed to open", L"Error", MB_ICONERROR);
		return 0;
	}

	ShowWindow(hDashboard, window_mode);  
	
	//===============================================
	// 	MESSAGE LOOP
	//===============================================
	MSG event = {};
	while(GetMessage(&event, NULL, 0, 0) > 0) {
		TranslateMessage(&event);
		DispatchMessage(&event);
	}	

	return 0;		
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam) {
	switch (event) {
		case WM_PAINT: {

			// Background painting
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
			break;
		}
	}
	return DefWindowProc(hWnd, event, wParam, lParam);
}

