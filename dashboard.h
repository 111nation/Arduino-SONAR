#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

// Libraries
#include "FontLib/font.h"

#define WINDOW_WIDTH 1000  
#define WINDOW_HEIGHT 500

// Colors
#define BACKGROUND_COLOR 0x000000
#define MID_COLOR 0x66666E
#define ACCENT_0 0xF4F4F6 // Lightest
#define ACCENT_1 0xE6E6E9
#define ACCENT_2 0x9999A1 // Darkest
			  
// =================
//  PAINTING DATA
// =================
class Paint {
	public:
	HDC area=nullptr;
	HBRUSH hbrush=nullptr;
	
	int x=0; 
	int y=0; 
	int xend=0;
	int yend=0;

	COLORREF color = 0;
	Font * font;

	~Paint();
};
