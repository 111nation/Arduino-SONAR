#include "display.h"

namespace display {
	Paint paint;
	circle arrCircles [CIRCLES];	
}

using namespace display;

void SonarDisplay(HWND hWnd) {
	paint.area = GetDC(hWnd);

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
	

	

	ReleaseDC(hWnd, paint.area);
}


