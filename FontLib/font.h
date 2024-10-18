#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

class Font {
	public:
	// Font Properties
	int size = 24;
	DWORD italics = FALSE;
	DWORD underline = FALSE;
	int weight = FW_NORMAL;

	Font(std::string font);
	~Font();	

	HFONT Use();
	void Reset(); 

	private:
	std::string font_name="";
 	HFONT hFont=nullptr;

	bool Valid();	
	void DefaultFont();
};
