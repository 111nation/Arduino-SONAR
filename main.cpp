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

	return 0;		
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT event, WPARAM wParam, LPARAM lParam) {
	switch (event) {
		case WM_CREATE: {
			Paint paint;
			paint.hWnd = hWnd;
			paint.Transparency();
			break;
		}

		case WM_PAINT: {
			//=====PAINTING DATA========
			Paint paint;
			PAINTSTRUCT ps;
			paint.area = BeginPaint(hWnd, &ps);
			const int window_corner = 25;
//			int margin;

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
			paint.border.color = RGB(67, 74, 88);
			
			paint.RoundRect(window_corner);

			//=========TITLE BAR========
			paint.Reset();
			paint.x = 0;
			paint.y = 0;
			paint.xend = WINDOW_WIDTH;
			paint.yend = TITLE_BAR-1;
			paint.color = 0;
			paint.border.width = 1;
			paint.border.color = RGB(67, 74, 88);

			paint.RoundRect(window_corner);
			
			paint.border.width = 0;
			paint.y = (int) window_corner/2;
			paint.yend = TITLE_BAR;
			paint.x = 1;
			paint.xend = WINDOW_WIDTH;

			paint.Rectangle();	

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
				if (mouse.x >= 0 && mouse.x <= WINDOW_WIDTH) {
					SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
				}
			}

		}
	}
	return DefWindowProc(hWnd, event, wParam, lParam);
}

