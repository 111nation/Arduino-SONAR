#pragma once

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <sstream>

// Libraries
#include "FontLib/font.h"
#include "paint.h"

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
#define ACCENT_2 0x9999A1 // Darkest

//=====TITLE BUTTONS DATA=======
const int button_width = 0.03 * WINDOW_WIDTH;	
const int button_height = TITLE_BAR;
const int icon_width = 1;

//=====WINDOW DATA============
const COLORREF border_color = GetSysColor(COLOR_WINDOWFRAME);
const int window_corner = 20;

void Paint_Minimize(HWND hWnd, bool bHover);
void Paint_Exit(HWND hWnd, bool bHover);
