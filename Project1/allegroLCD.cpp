#include "allegroLCD.h"
#include <iostream>

using namespace std;

allegroLCD::allegroLCD() {
	if (!al_init()) {
		initError = true;
		cout << "Couldn't initialize allegro (lcd emulation)" << endl;
	}
	if (!al_init_font_addon()) {
		initError = true;
		cout << "Couldn't initialize allegro font addon (lcd emulation)" << endl;
	}
	if (!al_init_ttf_addon()) { // necesario para manejo de fuentes de letras
		cout << "Couldn't initialize allegro font addon (lcd emulation)" << endl;
		initError = true;
	}
	ledFont = al_load_font("dotty.ttf", 100, NULL);
	if (!ledFont) {
		initError = true;
		cout << "Couldn't fetch font dotty.ttf (lcd emulation)" << endl;
	}

	display = al_create_display(600, 300);

	if (!display) {
		initError = true;
		cout << "Couldn't create display (lcd emulation)" << endl;
	}
	else {
		refreshDisplay();
	}

	cadd = 1;
}

allegroLCD::~allegroLCD() {
	
	if (ledFont) al_destroy_font(ledFont);
	al_shutdown_font_addon();
	if (display)al_destroy_display(display);
	cout << "Deallocated lcd resources!" << endl;
}

bool allegroLCD::lcdInitOK() { return !initError; }

bool allegroLCD::lcdMoveCursorUp() { 
	if (cadd > 16) {
		cadd -= 16;
		return true;
	}
	else {
		return false;
	}
}

bool allegroLCD::lcdMoveCursorDown() { 
	if (cadd < 16) {
		cadd += 16;
		return true;
	}
	else {
		return false;
	}
}

bool allegroLCD::lcdMoveCursorRight() {
	if (cadd != 16 && cadd != 32) {
		cadd += 1;
		return true;
	}
	else {
		return false;
	}
}

bool allegroLCD::lcdMoveCursorLeft() {
	if (cadd != 0 && cadd != 33) {				//decia 33 pero pude 32.esta bien???
		cadd -= 1;
		return true;
	}
	else {
		return false;
	}
}

bool allegroLCD::clearPosition(int currentPos) {
	if (currentPos < 32 && currentPos >= 0) {
		int line = currentPos / 16;
		int col = currentPos % 16;
		content[line][col] = ' ';
		return true;
	}
	else return false;
}

bool allegroLCD::lcdClearToEOL() {
	int line = (cadd-1) / 16;
	int allOk;
	int aux_cadd = cadd-1;
	switch (line) {
		case 0:
			for (; aux_cadd < 16; aux_cadd++) {
;				clearPosition(aux_cadd);
			}
			allOk = true;
			break;
		case 1:
			for (; aux_cadd < 32; aux_cadd++) {
				clearPosition(aux_cadd);
			}
			allOk = true;
			break;
		default:
			allOk = false;
			break;
	}
	return allOk;
}

bool allegroLCD::refreshDisplay() {
	al_clear_to_color(al_color_name("black"));
	al_draw_text(ledFont,
		font_color, 50, 40, 0,
		(content[0]).c_str());
	al_draw_text(ledFont,
		font_color, 50, 160, 0,
		content[1].c_str());
	al_flip_display();
	return true;

}

bool allegroLCD::lcdClear() {
	int aux_cadd;
	bool success;
	for (aux_cadd = 0; aux_cadd < 32; aux_cadd++) {
		success = clearPosition(aux_cadd);
	}
	cadd = 1;
	return success;	
}

bool allegroLCD::lcdSetCursorPosition(const cursorPosition& po) {
	if (po.row <= 2 && po.column <= 16 && po.row > 0 && po.column > 0) {
		cadd = (po.row-1) * 16 + po.column;
		return true;
	}
	else return false;
}

cursorPosition allegroLCD::lcdGetCusorPosition() {
	int line = ((cadd-1) / 16)+1;
	int col = (cadd-1) % 16;
	cursorPosition currPos = { line, col };
	return currPos;
}

allegroLCD& allegroLCD::operator<<(const unsigned char c) {
	
	int currentPos = cadd - 1;
	int line = currentPos / 16;
	int col = currentPos % 16;
	content[line][col] = static_cast<char>(c);
	if (lcdMoveCursorRight()) {}
	else if (lcdMoveCursorDown()) {}
	else cadd = 1;
	refreshDisplay();
	return *this;
}

allegroLCD& allegroLCD::operator<<(const unsigned char* c) {
	string message(reinterpret_cast<const char*>(c));
	int currentPos = cadd - 1;
	int line = currentPos / 16;
	int col = currentPos % 16;
	int space_left = 16 - col;
	int start_drawing;
	size_t size;
	if (space_left >= message.size()) {
		start_drawing = 0;
		size = message.size();
	}
	else {
		start_drawing = static_cast<int>(message.size()) - space_left;
		size = space_left;
	}
	content[line].replace(col, message.size(), message.substr(start_drawing, size));
	refreshDisplay();
	cadd = cadd += static_cast<int>(size);
	return *this;

}

void allegroLCD::rest(double time) {
	al_rest(time);
}

bool allegroLCD::lcdGetError() { return false; }


