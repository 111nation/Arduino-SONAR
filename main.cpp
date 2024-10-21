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
			const COLORREF border_color = GetSysColor(COLOR_WINDOWFRAME);
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


			//=====TITLE BUTTONS=======
			const int button_width = 0.03 * WINDOW_WIDTH;	
			const int button_height = TITLE_BAR;
			
			// MINIMIZE
			const int icon_width = 1;
			paint.Reset();
			paint.x = WINDOW_WIDTH - button_width*2;
			paint.y = 1;
			paint.xend = paint.x + button_width;
			paint.yend = button_height;
			paint.color = RGB(44, 46, 50);
		       	paint.Rectangle();	

			paint.x = paint.x + 9;
			paint.xend = paint.xend - 9;
			paint.y = (int) button_height/2;
			paint.yend = (int) button_height/2;
			paint.border.color = RGB(255, 255, 255);
			paint.border.width = icon_width;
			paint.Line();

			//EXIT
			paint.Reset();
			paint.border.width = 1;
			paint.border.color = border_color;
			paint.color = RGB(242, 63, 66);		
			paint.y = 0;
			paint.yend = button_height-1;
			paint.x = WINDOW_WIDTH - button_width-1; // Perfect square
			paint.xend = WINDOW_WIDTH;

			paint.RoundRect(window_corner);

			// Make left and bottom flat rectangle
			paint.y = 1;
			paint.yend = button_height;
			paint.border.width = 0;
			paint.xend = WINDOW_WIDTH - (button_width / 2);
			paint.Rectangle();	

			paint.xend = WINDOW_WIDTH;
			paint.y = button_height / 2;
			paint.Rectangle();

			// X
			// \*
			const int x_cross = WINDOW_WIDTH - (button_width / 2) - 1;
			const int y_cross = button_height/2;
			paint.x = x_cross - 5;
			paint.y = y_cross - 5;
			paint.xend = x_cross + 6;
			paint.yend = y_cross + 6;
			paint.border.color = RGB(255,255,255);
			paint.border.width = icon_width;

			paint.Line();
			
			// */
			paint.x = x_cross - 5;
			paint.y = y_cross + 5;
			paint.xend = x_cross + 6;
			paint.yend = y_cross - 6;

			paint.Line();

			//=====TITLE TEXT========
			paint.font.name = "Montserrat";
			paint.font.size = 20;
			paint.font.weight = 500;
			paint.x = 20;
			paint.y = TITLE_BAR / 2;
			paint.color = RGB(255, 255, 255);

			paint.Text("SONAR");


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

