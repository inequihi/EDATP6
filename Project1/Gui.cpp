#include "Gui.h"


Gui::Gui()
{
	if (AllegroInit() && ImguiInit())
	{
		timer = al_create_timer(1/10.0);
		this->queue = al_create_event_queue();
		this->timer_queue = al_create_event_queue();
		al_register_event_source(this->queue, al_get_display_event_source(display));
		al_register_event_source(this->queue, al_get_mouse_event_source());
		al_register_event_source(this->queue, al_get_keyboard_event_source());
		al_register_event_source(this->timer_queue, al_get_timer_event_source(timer));
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		Cargando = "Cargando";  //aca meter los cambios para que se vayan agregando y sacando puntos para que simule que carga
		newSearch = false;
		settingUp = true;
		close = false;
		descargandoTweets = true;
		currentTweet = 0;
		currentPos = 0;
	}
	else
		settingUp = false;
}


Gui::~Gui()
{

	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();

	if (timer_queue)
		al_destroy_event_queue(timer_queue);
	if(queue)
		al_destroy_event_queue(queue);
	if (display)
		al_destroy_display(display);
	if (timer)
		al_destroy_timer(timer);
	
	
}

void Gui::startGUI()
{

	int times = 0;

	do {
		
		while (al_get_next_event(queue, &ev)) {
			ImGui_ImplAllegro5_ProcessEvent(&ev);
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				close = true;
			}
		}
		if (settingUp)
		{
			ImGui_ImplAllegro5_NewFrame();
			ImGui::NewFrame();
			print_gui_setup(); 
			ImGui::Render();
			al_draw_scaled_bitmap(background,
				0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
				0, 0, SIZE_SCREEN_X, SIZE_SCREEN_Y, 0);
			ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
			al_flip_display();
			
			if (!settingUp) {
				cantTw = (cantTw == 0) ? cantTw=10 : cantTw;
				my_client = Client(userTw.c_str(), cantTw);
				if (my_client.GetToken()) {

					myLCD =make_unique<allegroLCD>();

					while (descargandoTweets) {
						cargando();
						descargandoTweets=my_client.GetTweets();
					}
					if (my_client.numberTweetsCollected()) {
						currentPos = 0;
						currentTweetData = "                   " + my_client.returnTweet(currentTweet, cantTw) + "                ";
						currentTweetDate = my_client.returnDate(currentTweet, cantTw);
						al_start_timer(timer);
					}
					else{						
						errorMessage();
					}				
				}				
			}
		}
		else
		{
			// Ya tengo usuario -> uso CLIENT Y LED
			ImGui_ImplAllegro5_NewFrame();
			ImGui::NewFrame();

			print_gui_controls();

			ImGui::Render();
			al_set_target_backbuffer(display);
			al_draw_scaled_bitmap(background,
				0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
				0, 0, SIZE_SCREEN_X, SIZE_SCREEN_Y, 0);

			//Evento timer nos indica que debemos imprimir los tweets
			if (al_get_next_event(timer_queue, &timerev))
			{
				showTweet();
			}
			
			al_set_target_backbuffer(display);
			ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
			
			al_flip_display();

		}
		
	} while (!close);
	
}


void Gui::print_gui_setup()
{	
	if (newSearch)
	{
		al_set_target_backbuffer(display);
		descargandoTweets = true;

	}
	ImGuiWindowFlags window_flags = 0;
	
	window_flags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowPos(ImVec2(250, 50));
	ImGui::SetNextWindowSize(ImVec2(450, 100));

	ImGui::Begin("Seleccione usuario y la cantidad de twitts a buscar", 0, window_flags);	
	static char user[MAX_TWITTER_NAME];
	ImGui::InputText("Usuario", user, sizeof(char) * MAX_TWITTER_NAME);

	ImGui::SliderInt("Cantidad de Twitts", &cantTw, 1, 50);
	if (ImGui::Button("Buscar"))
	{
		settingUp = false;
		userTw.assign(user);
		std::cout << userTw << std::endl; //guarda bien nuevo usuaro
	}
	ImGui::End();
}

void Gui::print_gui_controls() {

	ImGuiWindowFlags window_flags = 0;
	
	window_flags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowPos(ImVec2(250, 50));
	ImGui::SetNextWindowSize(ImVec2(150, 100));
	ImGui::Begin("Seleccione Tweet", 0, window_flags);
	
	if (ImGui::Button("Last tw"))
	{
		if (currentTweet > 0) {
			currentTweet--;
		}
		else {
			currentTweet = cantTw-1;
		} 
		currentTweetData = "                   " + my_client.returnTweet(currentTweet, cantTw) + "                ";
		currentTweetDate =  my_client.returnDate(currentTweet, cantTw) ;
		showTweet();
		currentPos = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Next tw"))
	{	
		if (currentTweet < (cantTw-1)) {
			currentTweet++;
			
		}
		else {
			currentTweet = 0;
		}
		currentTweetData = "                   " + my_client.returnTweet(currentTweet, cantTw) + "                ";
		currentTweetDate = my_client.returnDate(currentTweet, cantTw);
		showTweet();
		currentPos = 0;
	}
	ImGui::Text("Tweet :%d/%d", currentTweet+1, cantTw);

	ImGui::End();
}


void Gui::showTweet() {

	
	cursorPosition DatePos = { 1,1 };



	if ((currentPos <= (currentTweetData.size()-16))) {

		cursorPosition DataPos = { 2, 1 };

		myLCD->lcdClear();
		myLCD->lcdSetCursorPosition(DatePos);
		*myLCD << reinterpret_cast<const unsigned char*>(currentTweetDate.c_str());

		myLCD->lcdSetCursorPosition(DataPos);
		*myLCD << reinterpret_cast<const unsigned char*>(currentTweetData.substr(currentPos, 16).c_str());

		currentPos++;


	}
	else {
		currentPos = 0;
	}
	
}

void Gui::cargando() {					//cargando twwets
	string dots = "";
	string userTW = "@";
	userTW.append(userTw);
	cursorPosition UserPos = { 1,1 };
	myLCD->lcdSetCursorPosition(UserPos);
	*myLCD << reinterpret_cast<const unsigned char*>(userTW.c_str());

	cursorPosition LoadPos = { 2,1 };
	myLCD->lcdSetCursorPosition(LoadPos);
	
	static int counter = 0;
	if (!(counter % 50))
	{
		if (Cargando.size() < (size_t)16)
			Cargando += ".";
		else
		{
			Cargando.clear();			          
			Cargando.assign("Cargando");
		}

	}
	*myLCD << reinterpret_cast<const unsigned char*>(Cargando.c_str());

	counter++;

}

void Gui::errorMessage() {
	myLCD->lcdClear();
	myLCD->lcdSetCursorPosition({ 1,1 });
	*myLCD << reinterpret_cast<const unsigned char*>("Error getting");
	myLCD->lcdSetCursorPosition({ 2,1 });
	*myLCD << reinterpret_cast<const unsigned char*>("Tweets");
	settingUp = true;
	newSearch = true;
}
bool Gui::AllegroInit()
{
	if (al_init())
	{
		if (al_init_image_addon())
		{
			if (al_install_mouse())
			{
				if (al_install_keyboard())
				{
					if (al_init_primitives_addon())
					{
						background = al_load_bitmap("twitterbac.png");
						return true;
					}
					else
						printf("ERROR al_init_primitives_addon");
				}
				else
					printf("ERROR al_instal_keyboard\n");
			}
			else
				printf("ERROR al_install_mouse\n");
		}
		else
			printf("ERROR al_init_image_addon\n");
	}
	else
		printf("ERROR al_init\n");
	return false;
}


bool Gui::ImguiInit(void)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls


	display = al_create_display(SIZE_SCREEN_X, SIZE_SCREEN_Y);
	al_set_window_position( display, 500 , 100); //posicion del menu
	if (!display)
		return false;

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();


	// Setup Platform/Renderer bindings
	ImGui_ImplAllegro5_Init(display);

	return true;
}
