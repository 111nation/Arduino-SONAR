#include "dashboard.h"

Paint::~Paint() {
	if (font != nullptr) {
		delete font;
	}

	if (hbrush) {
		DeleteObject(hbrush);
		hbrush = NULL;
	}

}

void Paint::RoundRect(int corner_radius) {
	this->RoundRect(corner_radius, corner_radius);
}

void Paint::RoundRect(int cornerx, int cornery) {
	ApplyColor();

	const int x = pos.left;
	const int y = pos.top;
	const int xend = pos.right;
	const int yend = pos.bottom;
	
	::RoundRect(area, x, y, xend, yend, cornerx, cornery);

	DeleteObject(hbrush);
}

void Paint::FillRect() {
	ApplyColor();

	::FillRect(area, &pos, hbrush);

	DeleteObject(hbrush);
}

//====================
// UTILITIES
//====================
void Paint::ApplyColor() {
	hbrush = CreateSolidBrush(color);
	SelectObject(area, hbrush); // Colours 

}

