#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

class Font {
	public:
	// Font Properties
	std::string name="";
	int size = 24;
	DWORD italics = FALSE;
	DWORD underline = FALSE;
	int weight = FW_NORMAL;

	~Font();	

	HFONT Use();
	void Reset(); 

	private:
 	HFONT hFont=nullptr;

	bool Valid();	
	void DefaultFont();
};
