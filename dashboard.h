#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <sstream>

// Libraries
#include "FontLib/font.h"


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

bool hover_minimize = false;
bool hover_maximize = false;

//=====TITLE BUTTONS DATA=======
const int button_width = 0.03 * WINDOW_WIDTH;	
const int button_height = TITLE_BAR;
const int icon_width = 1;

//=====WINDOW DATA============
const COLORREF border_color = GetSysColor(COLOR_WINDOWFRAME);
int window_corner = 20;

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

	Font font;

	~Paint();
	void Reset();
	void RECTtoPos(RECT rect);

	//====PAINT UTILS====
	void Line();
	void Rectangle();
	void RoundRect(int corner_radius);
	void RoundRect(int cornerx, int cornery);
	void Text(const std::string text);
	void Transparency(int color, BYTE alpha); 
	void Transparency();
};


// OTHER PAINTING
void Paint_Minimize(HWND hWnd, bool bHover) {
	Paint paint; 
	paint.area = GetDC(hWnd);
	
	// MINIMIZE
	paint.Reset();
	paint.x = WINDOW_WIDTH - button_width*2;
	paint.y = 1;
	paint.xend = paint.x + button_width;
	paint.yend = button_height;
	paint.color = RGB(49, 51, 56);
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
	Paint paint;
	paint.area = GetDC(hWnd);
	//EXIT
	paint.Reset();
	paint.border.width = 1;
	paint.border.color = border_color;
	paint.color = RGB(242, 63, 66);		
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
