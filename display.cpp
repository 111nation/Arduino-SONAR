#include "display.h"
#include <math.h>

namespace display {
		Paint paint;
		circle arrCircles [CIRCLES];
};

#define PI 2 * acos(0.0)

using namespace display; 

void RadiiXY(int centerx, int centery, double deg,  int * x, int * y);

void SonarDisplay(HWND hWnd, double deg) {
	paint.ClearDC();
	const COLORREF transparent_color = RGB(0,0,0);

	// Double buffer painting
	HDC hDC = GetDC(hWnd);
	// Creates buffer
	paint.area = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, WINDOW_WIDTH, WINDOW_HEIGHT+TITLE_BAR);
	SelectObject(paint.area, hBitmap);
	
	// Manages transparency by creating transparent white background which acts as a 'white (green) screen'
	paint.Reset();
	paint.x = 0;
	paint.y = 0;
	paint.xend = WINDOW_WIDTH;
	paint.yend = WINDOW_HEIGHT + TITLE_BAR;
	paint.color = transparent_color;
	
	paint.Rectangle();

	paint.Reset();

	// Stores how many circles to create and their corresponding data
	circle arrCircles [CIRCLES] = {0};
	
	//======BLACK CIRCLE BACKGROUND=============
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
		
	
	DegreesText();
	Nav(deg);
	
	// Transfer buffer onto window
	TransparentBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT+TITLE_BAR,
			paint.area, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT+TITLE_BAR,
			transparent_color); 

	//BitBlt(hDC, 0, TITLE_BAR, WINDOW_WIDTH, WINDOW_HEIGHT+TITLE_BAR, paint.area, 0, TITLE_BAR, SRCCOPY); 

	DeleteObject(hBitmap);
	DeleteDC(paint.area);
	ReleaseDC(hWnd, hDC);
}

void DegreesText() {
	// Text styling
	paint.Reset();
	paint.font.size = 20;
	paint.color = ACCENT_3;
	paint.font.name = "Montserrat";
	paint.font.weight = 500;
	//paint.font.italics = TRUE;
	
	// Text display
	int CHAR_WIDTH = 4;

	// MAIN 0; 90; 180; 270; 360
	std::string text = "90";
	paint.x = (int) (SONAR_DISPLAY.x + SONAR_DISPLAY.xend)/2 - (CHAR_WIDTH * text.length());
	paint.y = (int) SONAR_DISPLAY.y - TEXT_MARGIN;
	paint.Text(text);

	text = "270";
	paint.x = (int) (SONAR_DISPLAY.x + SONAR_DISPLAY.xend)/2 - (CHAR_WIDTH * text.length());
	paint.y = (int) SONAR_DISPLAY.yend;
	paint.Text(text);

	text = "0";
	paint.x = (int) (SONAR_DISPLAY.x - TEXT_MARGIN) - (CHAR_WIDTH * text.length());
	paint.y = (int) (SONAR_DISPLAY.y + SONAR_DISPLAY.yend - TITLE_BAR) / 2;
	paint.Text(text);

	text = "180";
	paint.x = (int) (SONAR_DISPLAY.xend) + (CHAR_WIDTH * text.length());
	paint.Text(text);
}

void Nav(double deg) {
	paint.Reset();
	
	struct Nav { 
		int x = 0;
		int y = 0;
		int xend = 0;
		int yend = 0;
	};

	const Nav NAV {
		SONAR_DISPLAY.x,
		SONAR_DISPLAY.y,
		(int) ((SONAR_DISPLAY.x + SONAR_DISPLAY.xend) / 2) - 1 ,
		(int) ((SONAR_DISPLAY.y + SONAR_DISPLAY.yend) / 2) - 1
	};
	
	//===NAVIGATION ARC===
	paint.Reset();
	paint.color = arrColors[CIRCLES-1];
	paint.x = NAV.x+1;
	paint.y = NAV.y+1;
	paint.xend = SONAR_DISPLAY.xend-1;
	paint.yend = SONAR_DISPLAY.yend-1;
	
	int xrad1 = NAV.x + 1;
	int yrad1 = NAV.yend - 1;
	int xrad2 = 0;
	int yrad2 = 0;
	
	// Figures out starting radial
	RadiiXY(NAV.xend, NAV.yend, deg-20, &xrad1, &yrad1);
	// Figures out ending radial
	RadiiXY(NAV.xend, NAV.yend, deg, &xrad2, &yrad2);
		

	paint.Pie(xrad1, yrad1, xrad2, yrad2, false);
	
	//====NAVIGATION PIN====
	paint.border.color = arrColors[CIRCLES];
	paint.border.width = 4;
	paint.xend = NAV.xend;
	paint.yend = NAV.yend;

	// Calculates start & end pos
	RadiiXY(paint.xend, paint.yend, deg, &paint.x, &paint.y);

	paint.Line();	
}

void RadiiXY(int centerx, int centery, double deg,  int * x, int * y) {
	// Recieve degrees in degrees, convert to radiants
	const double RAD = (deg-180) * (PI / 180.0);		
	
	// Gets x,y value which lies on the circumference	
	*x = centerx + (int) ((RADII-RADII_MARGIN) * cos(RAD));
	*y = centery + (int) ((RADII-RADII_MARGIN) * sin(RAD));
}

