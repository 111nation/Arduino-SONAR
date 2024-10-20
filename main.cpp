#include "dashboard.h"

const std::wstring MAIN_CLASS = L"MAIN_CLASS";

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

	// Set Transparency of window
	SetLayeredWindowAttributes(hDashboard, TRANSPARENT_COLOR, 0, LWA_COLORKEY);
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
			//=====PAINTING DATA========
			Paint paint;
			PAINTSTRUCT ps;
			paint.area = BeginPaint(hWnd, &ps);
			const int window_corner = 25;
			
			//=====TRANSPARENCY=========
			paint.pos.left = 0;
			paint.pos.top = 0;
			paint.pos.right = WINDOW_WIDTH;
			paint.pos.bottom = WINDOW_HEIGHT + TITLE_BAR; 
			paint.color = TRANSPARENT_COLOR;

			paint.FillRect();
			
			//=====BACKGROUND===========
			paint.pos = ps.rcPaint;
			paint.color = BACKGROUND_COLOR;
			
			paint.RoundRect(window_corner);	
						
			//====TITLE BAR=============
			paint.pos.left = 0;
			paint.pos.top = 0;
			paint.pos.right = WINDOW_WIDTH;
			paint.pos.bottom = TITLE_BAR;
			paint.color = MID_COLOR;

			paint.RoundRect(window_corner);
		
			paint.pos.top = 15;
			paint.FillRect();
	

			EndPaint(hWnd, &ps);
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

