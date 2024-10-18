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

