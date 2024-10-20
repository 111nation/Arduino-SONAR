#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

// Libraries
#include "FontLib/font.h"


#define WINDOW_WIDTH 1000  
#define TITLE_BAR 20
#define WINDOW_HEIGHT 500

// Colors
#define DEF_TRANSPARENT RGB(254, 254, 254)
#define BACKGROUND_COLOR RGB(30, 31, 34) 
#define MID_COLOR RGB(43, 45, 49) 
#define ACCENT_0 0xF4F4F6 // Lightest
#define ACCENT_1 0xE6E6E9
#define ACCENT_2 0x9999A1 // Darkest

// =================
//  PAINTING DATA
// =================
class Paint {
	private:
	void UpdateColor();
	HBRUSH hbrush=NULL;
	HPEN hpen=NULL;
	
	public:
	HDC area=NULL;
	HWND hWnd=NULL;
	
	int x = 0;
	int y = 0;
	int xend = 0;
	int yend = 0;

	struct Corner {
		int x = 0;
		int y = 0;
	};
	Corner corner;

	COLORREF color = 0;

	struct Border{
		int width = 0;
		int color = 0;
		int style = PS_SOLID;
	};
	Border border;

	~Paint();
	void Reset();
	void RECTtoPos(RECT rect);

	//====PAINT UTILS====
	void Rectangle();
	void RoundRect(int corner_radius);
	void RoundRect(int cornerx, int cornery);
	void Transparency(int color, BYTE alpha); 
	void Transparency();

	Font * font = NULL;
};
