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
	void showTweet();
	void cargando();
	void errorMessage();
	void print_gui_setup();
	void print_gui_controls();
	bool ImguiInit();
	bool AllegroInit();
	int cantTw;
	int currentTweet;
	int currentPos;
	string Cargando;
	string userTw;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* background;
	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_EVENT_QUEUE* timer_queue;
	ALLEGRO_EVENT timerev;
	ALLEGRO_TIMER* timer;
	bool settingUp;
	bool close;
	bool newSearch;
	bool descargandoTweets;
	int myTicks;
	int tickCount;

	ImVec4 clear_color;
	unique_ptr<basicLCD>myLCD;
	Client my_client;
	string currentTweetData;
	string currentTweetDate;
};

