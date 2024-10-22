#pragma once

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Libraries
#include "FontLib/font.h"

#define DEF_TRANSPARENT RGB(254, 254, 254)

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
