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

void Paint_SonarDisplay(HWND hWnd) {
	const int display_width = 450;
	const int display_height = 450;
	
	Paint paint;
	paint.area = GetDC(hWnd);

	// Circle Data
	struct circle {
		int x = 0;
		int y = 0;
		int xend = 0;
		int yend = 0;
	};
	
	// Stores how many circles to create and their corresponding data
	const int CIRCLES = 4; 
	const int MARGIN = 15;
	const int PADDING = 20; // The start position of 1st circle
	circle arrCircles [CIRCLES] = {0};
	
	// Gradient colors
	const COLORREF arrColors[CIRCLES+1] = {
		RGB(9, 16, 95),
		RGB(21, 29, 120),
		RGB(43, 53, 160),
		RGB(53, 64, 189), //RGB(66, 77, 202),
		ACCENT_3
	};

	//======BLACK CIRCLE BACKGROUND=============
	// Constant defining start/ end positions of display
	const circle SONAR_DISPLAY  {
		/* X */	(int) (WINDOW_WIDTH - display_width)/2 + sonar_bg_margin,
		/* Y */	(int) (WINDOW_HEIGHT - display_height)/2 + TITLE_BAR,
		/*XEND*/(int) (WINDOW_WIDTH + display_width)/2 + sonar_bg_margin,
		/*YEND*/(int) (WINDOW_HEIGHT + display_height)/2 + TITLE_BAR
	};
	
	paint.x = SONAR_DISPLAY.x;
	paint.y = SONAR_DISPLAY.y;
	paint.xend = SONAR_DISPLAY.xend;
	paint.yend = SONAR_DISPLAY.yend;
	paint.color = ACCENT_2;
	paint.Circle();

	// CREATES INNER CIRCLES
	// Gradient effect
	paint.border.width = 0;	
	int circle_margin = PADDING - MARGIN;
	for (int circle=0; circle < CIRCLES; circle++) {
		circle_margin += MARGIN;
	
		paint.border.color = arrColors[circle + 1];	
				
		++paint.border.width;

		paint.x += circle_margin;
		arrCircles[circle].x = paint.x;
		
		paint.y += circle_margin;
		arrCircles[circle].y = paint.y;
		
		paint.xend -= circle_margin;
		arrCircles[circle].xend = paint.xend;
		
		paint.yend -= circle_margin;
		arrCircles[circle].yend = paint.yend;

		paint.Circle();

	}
	
	//========SONAR DECORATIONS==========
	// Loops create gradient effect
	// LINES
	paint.border.color = ACCENT_3;
	paint.border.width = 1;
	
	paint.x = (int) (SONAR_DISPLAY.x + SONAR_DISPLAY.xend)/2;
	paint.xend = paint.x;

	int line_length = PADDING;	
	for (int circle = 0; circle < CIRCLES; circle++) { // x2 to iterate both sides
		line_length += MARGIN;


		paint.border.color = arrColors[circle + 1];	

		++paint.border.width;	
	
		// VERTICAL
		paint.x = (int) (SONAR_DISPLAY.x + SONAR_DISPLAY.xend)/2;
		paint.xend = paint.x;

		// Top line
		paint.y = arrCircles[circle].y;
		paint.yend = paint.y + line_length;
		paint.Line();
		// Bottom line
		paint.y = arrCircles[circle].yend;
		paint.yend = paint.y - line_length;
		paint.Line();

		// HORIZONTAL
		paint.y = (int) (SONAR_DISPLAY.y + SONAR_DISPLAY.yend)/2;
		paint.yend = paint.y;

		// Left line
		paint.x = arrCircles[circle].x;
		paint.xend = paint.x + line_length;
		paint.Line();
		// Right line
		paint.x = arrCircles[circle].xend;
		paint.xend = paint.x - line_length;
		paint.Line();
	}
	
	// OUTER LINES
	line_length = 30; 
	paint.border.width = 1;
	paint.border.color = arrColors[0];	
	// VERTICAL
	paint.x = (int) (SONAR_DISPLAY.x + SONAR_DISPLAY.xend)/2;
	paint.xend = paint.x;

	// Top line
	paint.y = arrCircles[0].y - PADDING - 5;
	paint.yend = paint.y + line_length;
	paint.Line();
	// Bottom line
	paint.y = arrCircles[0].yend + PADDING + 5;
	paint.yend = paint.y - line_length;
	paint.Line();

	// HORIZONTAL
	paint.y = (int) (SONAR_DISPLAY.y + SONAR_DISPLAY.yend)/2;
	paint.yend = paint.y;

	// Left line
	paint.x = arrCircles[0].x - PADDING - 5;
	paint.xend = paint.x + line_length;
	paint.Line();
	// Right line
	paint.x = arrCircles[0].xend + PADDING + 5;
	paint.xend = paint.x - line_length;
	paint.Line();
	

	

	ReleaseDC(hWnd, paint.area);
}

void Paint_Status(HWND hWnd, int status) {
	using namespace STATUS;
	COLORREF color = 0;
	// Gets color to use
	switch (status) {
		case DISCONNECTED:
			color = RGB(242, 63, 66);
			break;
		case OK: 
			color = RGB(49, 51, 56);
			break;
		case WARNING:
		default:
			color = RGB(240, 178, 50);
			break;		
	}	
	

	//======STATUS DRAWING==========
	Paint paint;
	paint.area = GetDC(hWnd);	

	paint.Reset();
	std::string text = "STATUS";
	int text_width = 7 * text.length();
	paint.x = (int) (sonar_bg_margin)/2 - text_width;
	paint.y = TITLE_BAR + 15;
	paint.color = RGB(213, 214, 220);
	paint.font.size = 25;
	paint.font.weight = 900;
	paint.Text(text);

	// STATUS BAR
	paint.Reset();
	const int STATUS_WIDTH = (int) sonar_bg_margin/2;
	const int STATUS_HEIGHT = 20;

	paint.x = (int) (sonar_bg_margin - STATUS_WIDTH)/2; 
	paint.y = TITLE_BAR + 50;
	paint.xend = (int) (sonar_bg_margin + STATUS_WIDTH)/2;
	paint.yend = paint.y + STATUS_HEIGHT;
	paint.color = color;

	paint.RoundRect(10);	

	ReleaseDC(hWnd, paint.area);
}
