#include "Client.h"
#include "allegroLCD.h"

int main(void) {

	Client client("lanacion", 10);

	basicLCD* myptr=new allegroLCD;

	if (client.GetToken()) {

		client.GetTweets();
		

	}

	delete myptr;
}