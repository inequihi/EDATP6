#pragma once
#include "Client.h"
#include "allegroLCD.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "imgui_files/imgui.h"
#include "imgui_files/imgui_impl_allegro5.h"


#define SIZE_SCREEN_X 900
#define SIZE_SCREEN_Y 300

class Gui
{
public:
	Gui();
	~Gui();
	void startGUI();


protected:
	void print_gui();
	bool ImguiInit();
	bool AllegroInit();
	int cantTw;
	string userTw;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* background;
	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* queue;
	bool settingUp;
	bool close;
	ImVec4 clear_color;
	std::unique_ptr<basicLCD>myLCD;
	Client my_client;
};

