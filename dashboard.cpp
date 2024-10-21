#include "dashboard.h"

Paint::~Paint() {
	if (hbrush) {
		DeleteObject(hbrush);
		hbrush = NULL;
	}

	if (area) {
		DeleteObject(area);
		hbrush = NULL;
	}

	if (hpen) {
		DeleteObject(hpen);
		hpen = NULL;
	}

}

void Paint::Line() {
	UpdateColor();

	MoveToEx(area, x, y, NULL);
	LineTo(area, xend, yend);
}

void Paint::RoundRect(int corner_radius) {
	this->RoundRect(corner_radius, corner_radius);
}

void Paint::RoundRect(int cornerx, int cornery) {
	UpdateColor();
	
	::RoundRect(area, x, y, xend, yend, cornerx, cornery);
}

void Paint::Rectangle() {
	UpdateColor();

	::Rectangle(area, x, y, xend, yend);
}

void Paint::Text(std::string text) {
	Text(text.c_str());
}

void Paint::Text(const char text) {
	UpdateColor();
	
	// Updates area to support font
	HFONT hOldFont;
	GetObject(area, 0, &hOldFont); 
	SelectObject(area, font.Use());
	
	TextOutA(area, x, y, &text, sizeof(text));
	
	// Retrieves previous state (old font)
	SelectObject(area, hOldFont);
	DeleteObject(hOldFont);
}

//====================
// UTILITIES
//====================
void Paint::UpdateColor() {
	DeleteObject(hbrush);
	DeleteObject(hpen);

	// Background color
	hbrush = CreateSolidBrush(color);
	SelectObject(area, hbrush); // Colours 
	
	// border color
	if (border.width == 0) {
		hpen = CreatePen(PS_NULL, 0, DEF_TRANSPARENT);
	} else {
		hpen = CreatePen(border.style, border.width, border.color);
	}
	SelectObject(area, hpen);

}

void Paint::RECTtoPos(RECT rect) {
	x = rect.left;
	y = rect.top;
	xend = rect.right;
	yend = rect.bottom;
}

void Paint::Reset() {
	x = 0;
	y = 0;
	xend = 0;
	yend = 0;
	
	border.width = 0;
	border.style = PS_SOLID;
	border.color = DEF_TRANSPARENT;

	color = 0;

	corner.x = 0;
	corner.y = 0;
	Transparency(DEF_TRANSPARENT, 0); 
}

void Paint::Transparency(int color, BYTE alpha) {
	SetLayeredWindowAttributes(hWnd, color, alpha, LWA_COLORKEY);
}

void Paint::Transparency() {
	Transparency(DEF_TRANSPARENT, 0);
}

