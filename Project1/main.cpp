#include "Client.h"
#include "allegroLCD.h"

int main(void) {

	Client client("lanacion", 10);

	//std::shared_ptr<basicLCD>my_lcd = make_shared<allegroLCD>();

	if (client.GetToken()) {

		client.GetTweets();
		
		
	}

	
}