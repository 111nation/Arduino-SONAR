#pragma once

#include "dashboard.h"

namespace display {

	// SONAR PAINTING DATA
	const int display_width = 450;
	const int display_height = 450;

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

	const double RADII = (SONAR_DISPLAY.yend - SONAR_DISPLAY.y) / 2.0; 
	const int RADII_MARGIN = 3;

	const int TEXT_MARGIN = 20; // Margin of degree text

	const int RADIAL_SPAN = 20; // in degrees

	// NAVIGATION DATA	
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


	//=====TRACKING PROXIMITY DATA=============
	const int MAX_DEG = 180 + 1;
	const int MAX_PROX = 300;
	// PROXIMITY		60,	70,	0	
	// COLOR OF READING	FAR, 	FAREST, CLOSEST

	// Fading away completly starts when proximity is farther than 30-45deg
	// Proximity Colours
	namespace PROX_COLORS {
	       	const COLORREF 
			_NEAR = RGB(218, 55, 60),
			_MID = RGB(180, 28, 33),
			_FAR = RGB(140, 0, 5);
	};
};

// Drawing functions
void SonarDisplay(HWND hWnd, double deg, double prox);
void DegreesText();
void Nav(double deg);
void Proximity(int deg, int prox);
void InitProx();
