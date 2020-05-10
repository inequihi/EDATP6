#pragma once
#include "basicLCD.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
using namespace std;

class allegroLCD : public basicLCD
{	
public:
	allegroLCD();
	~allegroLCD();
	bool lcdInitOK();
	bool lcdMoveCursorUp();
	bool lcdMoveCursorDown();
	bool lcdMoveCursorRight();
	bool lcdMoveCursorLeft();
	bool lcdClearToEOL();
	bool lcdClear();
	bool lcdSetCursorPosition(const cursorPosition& po);
	cursorPosition lcdGetCusorPosition();
	allegroLCD& operator<< (const unsigned char c);
	allegroLCD& operator<< (const unsigned char* c); //el input a mi solo me funciona si lo casteo explicitamente : reinterpret_cast<const unsigned char*> ("foo");
	bool lcdGetError();
	void rest(double time);

private: 
	string content[2] = {"                ", "                "};
	bool initError = false;
	ALLEGRO_DISPLAY* display = nullptr;
	ALLEGRO_FONT* ledFont = nullptr;
	ALLEGRO_COLOR font_color = al_color_name("green");
	bool clearPosition(int currentPosition);
	bool refreshDisplay();
};

