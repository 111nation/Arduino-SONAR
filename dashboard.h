#pragma once

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <sstream>

// Libraries
#include "PaintLib/paint.h"

#define WINDOW_WIDTH 1000  
#define TITLE_BAR 23
#define WINDOW_HEIGHT 500

// Colors
#define DEF_TRANSPARENT RGB(254, 254, 254)
#define BACKGROUND_COLOR RGB(30, 31, 34)
//#define MID_COLOR RGB(43, 45, 49) 
#define MID_COLOR RGB(43, 45, 49)
#define ACCENT_0 RGB(181, 186, 193) // Lightest
#define ACCENT_1 0xE6E6E9
#define ACCENT_2 RGB(4, 6, 8) // Darkest
#define ACCENT_3 RGB(88, 101, 242)

//=====TITLE BUTTONS DATA=======
const int button_width = 0.03 * WINDOW_WIDTH;	
const int button_height = TITLE_BAR;
const int icon_width = 1;

// POSITIONS
const int minimize_x = WINDOW_WIDTH - button_width*2;
const int minimize_xend = WINDOW_WIDTH - button_width; 
const int exit_x = minimize_xend;
const int exit_xend = WINDOW_WIDTH;

const int sonar_bg_margin = 100;


//=====WINDOW DATA============
const COLORREF border_color = GetSysColor(COLOR_WINDOWFRAME);
const int window_corner = 20;

void Paint_Minimize(HWND hWnd, bool bHover);
void Paint_Exit(HWND hWnd, bool bHover);

//=====STATUS DATA============
namespace STATUS {
	const int DISCONNECTED = 1;
	const int WARNING = 2;
	const int OK = 3;
}
void Paint_Status(HWND hWnd, int status);

