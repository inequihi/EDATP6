#pragma once
struct cursorPosition {
	int row;
	int column;
};


class basicLCD
{
public:
	virtual ~basicLCD() {};
	virtual bool lcdInitOK() = 0;
	virtual bool lcdMoveCursorUp() = 0;
	virtual bool lcdMoveCursorDown() = 0;
	virtual bool lcdMoveCursorRight() = 0;
	virtual bool lcdMoveCursorLeft() = 0;
	virtual bool lcdClearToEOL() = 0;
	virtual bool lcdGetError() = 0;
	virtual bool lcdClear() = 0;
	virtual basicLCD& operator<< (const unsigned char c) = 0;
	virtual basicLCD& operator<<(const unsigned char* c) = 0;
	virtual bool lcdSetCursorPosition(const cursorPosition& po) = 0;
	virtual cursorPosition lcdGetCusorPosition() = 0; 
	
protected:
	unsigned int cadd;
};





