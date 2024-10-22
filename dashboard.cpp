#include "dashboard.h"

void Paint_Minimize(HWND hWnd, bool bHover) {	

	COLORREF button_color;

	// Determines button color to use based on hovers state
	if (bHover) {
		button_color = RGB(49, 51, 56);
	} else {
		button_color = 0;
	}

	Paint paint; 
	paint.area = GetDC(hWnd);
		
	// MINIMIZE
	paint.Reset();
	paint.x = WINDOW_WIDTH - button_width*2;
	paint.y = 1;
	paint.xend = paint.x + button_width;
	paint.yend = button_height;
	paint.color = button_color;
      	paint.Rectangle();	

	paint.x = paint.x + 9;
	paint.xend = paint.xend - 9;
	paint.y = (int) button_height/2;
	paint.yend = (int) button_height/2;
	paint.border.color = ACCENT_0;
	paint.border.width = icon_width;
	paint.Line();

	ReleaseDC(hWnd, paint.area);
}

void Paint_Exit(HWND hWnd, bool bHover) {
	
	COLORREF button_color;

	// Determines button color to use based on hovers state
	if (bHover) {
		button_color = RGB(242, 63, 66);
	} else {
		button_color = 0;
	}


	Paint paint;
	paint.area = GetDC(hWnd);
	//EXIT
	paint.Reset();
	paint.border.width = 1;
	paint.border.color = border_color;

	paint.color = button_color;		

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
	paint.border.color = ACCENT_0;
	paint.border.width = icon_width;

	paint.Line();
	
	// */
	paint.x = x_cross - 5;
	paint.y = y_cross + 5;
	paint.xend = x_cross + 6;
	paint.yend = y_cross - 6;

	paint.Line();

	ReleaseDC(hWnd, paint.area);
}

