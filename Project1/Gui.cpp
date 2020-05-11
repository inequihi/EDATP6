#include "Gui.h"


Gui::Gui()
{
	if (AllegroInit() && ImguiInit())
	{
		this->queue = al_create_event_queue();
		al_register_event_source(this->queue, al_get_display_event_source(display));
		al_register_event_source(this->queue, al_get_mouse_event_source());
		al_register_event_source(this->queue, al_get_keyboard_event_source());
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		settingUp = true;
		close = false;
	}
	else
		settingUp = false;
}


Gui::~Gui()
{
//	if (background)
	//	al_destroy_bitmap(background);
	if (display)
		al_destroy_display(display);
	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
}

void Gui::startGUI()
{
	close = false;
	int times = 0;
	bool firstTime = true;
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
			{print_gui(); }
			ImGui::Render();
			al_draw_scaled_bitmap(background,
				0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
				0, 0, SIZE_SCREEN_X, SIZE_SCREEN_Y, 0);
			ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
			al_flip_display();
		}
		else
		{
			// Ya tengo usuario -> uso CLIENT Y LED
			ImGui_ImplAllegro5_NewFrame();
			ImGui::NewFrame();

			//función de Gui de controles del led!!!!

			ImGui::Render();

			al_draw_scaled_bitmap(background,
				0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
				0, 0, SIZE_SCREEN_X, SIZE_SCREEN_Y, 0);

			ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
			al_flip_display();
			cantTw = (cantTw == 0) ? 10 : cantTw;
			if (firstTime) {
				my_client = Client(userTw.c_str(), cantTw);
				if (my_client.GetToken()) {
					my_client.GetTweets();
					//my_lcd = make_unique<allegroLCD>();
					//*my_lcd << reinterpret_cast<const unsigned char*> ("gegeg");
				}
				firstTime = false;
			}

			my_client.TweetsToLCD(1); //ejemplo com primer tweet
		}
		
	} while (!close);
}


void Gui::print_gui()
{	
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowPos(ImVec2(250, 50));
	ImGui::SetNextWindowSize(ImVec2(450, 100));

	ImGui::Begin("Seleccione usuario y la cantidad de twitts a buscar", 0, window_flags);
	static char user[MAX_TWITTER_NAME];

    //https://github.com/ocornut/imgui/issues/105*/

	ImGui::InputText("Usuario", user, sizeof(char) * MAX_TWITTER_NAME);

	ImGui::SliderInt("Cantidad de Twitts", &cantTw, 1, 50);
	if (ImGui::Button("Buscar"))
	{
		settingUp = false;
		userTw.assign(user);
	}
	ImGui::End();

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
	if (!display)
		return false;

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();


	// Setup Platform/Renderer bindings
	ImGui_ImplAllegro5_Init(display);

	return true;
}