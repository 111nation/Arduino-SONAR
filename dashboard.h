#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

// Libraries
#include "FontLib/font.h"


#define WINDOW_WIDTH 1000  
#define TITLE_BAR 30
#define WINDOW_HEIGHT 500

// Colors
#define TRANSPARENT_COLOR RGB(254, 254, 254)
#define BACKGROUND_COLOR 0x000000
#define MID_COLOR 0x66666E
#define ACCENT_0 0xF4F4F6 // Lightest
#define ACCENT_1 0xE6E6E9
#define ACCENT_2 0x9999A1 // Darkest
			  
// =================
//  PAINTING DATA
// =================
class Paint {
	private:
	void ApplyColor();

	public:
	HDC area=nullptr;
	HBRUSH hbrush=nullptr;
	
	RECT pos;
	struct Corner {
		int x = 0;
		int y = 0;
	};
	Corner corner;

	COLORREF color = 0;

	~Paint();

	//====PAINT UTILS====
	void FillRect();
	void RoundRect(int corner_radius);
	void RoundRect(int cornerx, int cornery);


	Font * font = nullptr;
};
