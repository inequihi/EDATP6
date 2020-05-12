#include "Client.h"

Client::Client()
{
	usuario = "";
	numTweets = 0;
	query = "";
}



Client::Client(std::string usuario_, int nTweets_) {

	usuario = usuario_;
	numTweets = nTweets_;
	query += usuario + "&count=" + std::to_string(numTweets) ; 
}

bool Client::GetToken() {

	bool result = true;

	curl = curl_easy_init();
	if (curl)
	{
		//Seteamos primero la pagina donde nos vamos a conectar. Para buscar el token es siempre la misma
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");

		// Si la página nos redirije a algún lado, le decimos a curl que siga dicha redirección.
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		// Le decimos a CURL que trabaje tanto con HTTP como HTTPS, Autenticación por HTTP en modo básico.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		//Preparamos el password para la autenticación
		std::string userPwd = API_key + ":" + API_SecretKey;
		//Se lo seteamos a CURL.
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());

		struct curl_slist* list = NULL;

		//Le decimos a CURL que vamos a mandar URLs codificadas y además en formato UTF8.
		list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Le decimos a CURL que trabaje con credentials.
		std::string data = "grant_type=client_credentials";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		//Le decimos a curl que cuando haya que escribir llame a myCallback y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		// Perform the request, res will get the return code
		codErr = curl_easy_perform(curl);

		// Nos fijamos si hubo algún error
		if (codErr != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(codErr) << std::endl;
			//Hacemos un clean up de curl antes de salir.
			curl_easy_cleanup(curl);
			result= false;
		}

		// Si no hubo errores hago un clean up antes de realizar un nuevo query.
		curl_easy_cleanup(curl);

		//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		//Abajo busco el token en el JSON para luego acceder a los twits.
		Jdata = json::parse(readString);

		try
		{
			//Tratamos de acceder al campo acces_token del JSON
			std::string aux = Jdata["access_token"];
			token = aux;
			//std::cout << "Bearer Token get from Twitter API: \n" << token << std::endl;
		}
		catch (std::exception& e)
		{
			//Si hubo algun error, se muestra el error que devuelve la libreria
			std::cerr << e.what() << std::endl;
			result= false;
		}
	}
	else
	{
		std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
		result= false;
	}

	return result;

}

bool Client::GetTweets() {

	static bool init = true;			//Inicializamos todo una sola vez
	
	if (init)
	{
		setopt();
		init = false; 
	}

	estado = true;
	if ((curl != NULL) & (multiHandle != NULL))
	{
		//Realizamos ahora un perform no bloqueante
		curl_multi_perform(multiHandle, &stillRunning);
		if (stillRunning)
		{
			//Debemos hacer polling de la transferencia hasta que haya terminado
			codMErr = curl_multi_perform(multiHandle, &stillRunning);
			if (codMErr != CURLE_OK)
			{
				//Cleanup 
				curl_easy_cleanup(curl);
				curl_multi_cleanup(multiHandle);
				std::cout << "ERROR EN RECUPERACION DE TWEETS" << std::endl;
				std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(codErr) << std::endl;
				return 0;
			}
			//processstate= true;

			std::cout << "....CARGANDO TWEETS POR FAVOR ESPERE...." << std::endl;
		}
		else
		{
			//Checkeamos errores
			if (codErr != CURLE_OK)
			{
				std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(codErr) << std::endl;
				//Hacemos un clean up de curl antes de salir.
				curl_easy_cleanup(curl);
				curl_multi_cleanup(multiHandle);
				return 0;
			}
			//Cleanup  final 
			curl_easy_cleanup(curl);
			curl_multi_cleanup(multiHandle);
			init = true;

			//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
			//con toda la informacion de los tweets que le pedimos
			Jdata = json::parse(readString);

			try
			{
				//Al ser el JSON un arreglo de objetos JSON se busca el campo text para cada elemento
				for (auto element : Jdata)
				{
					string data = boost::locale::conv::from_utf <char>(element["text"], "ISO-8859-15");
					string fecha = boost::locale::conv::from_utf <char>(element["created_at"], "ISO-8859-15");

					Tweet tempTweet(fecha, data, usuario);
					tempTweet.checkData();
					alltweets.push_back(tempTweet);
				}
			}
			catch (std::exception& e)
			{
				//Muestro si hubo un error de la libreria
				std::cerr << e.what() << std::endl;
			}
			estado = false;
		}
		return estado;
	}
	else
		std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
	return 0;
}

void Client::setopt(void)
{
	curl = curl_easy_init();
	multiHandle = curl_multi_init();
	readString = "";
	int stillRunning = 0;

	if ((curl != NULL) & (multiHandle != NULL))
	{
		//Attacheo el easy handle para manejar una coneccion no bloqueante.
		curl_multi_add_handle(multiHandle, curl);

		//Seteamos URL FOLLOWLOCATION y los protocolos a utilizar igual que antes.
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		//Construimos el Header de autenticacion como lo especifica la API
		//usando el token que obtuvimos antes
		struct curl_slist* list = NULL;
		std::string aux = "Authorization: Bearer ";
		aux = aux + token;
		list = curl_slist_append(list, aux.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Seteamos los callback igual que antes
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);
	}

}
//Funcion auxiliar para imprimir los tweets en pantalla una vez parseados
void Client::printTweets(std::vector<Tweet> tweets_)
{
	int i;
	for (i=0; i< tweets_.size() ; ++i)
	{
		std::cout << tweets_[i].GetFecha() << std::endl;
		std::cout << usuario << ": " << tweets_[i].GetData() << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
	}
}



//Concatena lo recibido en content a s
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	//fprintf(stdout, "%s",data);
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}

string Client::returnTweet(int index, int cantTweets) {
	if (index >= 0 && index < cantTweets) {
		return alltweets[index].GetData();
	}
	else {
		return alltweets[0].GetData();
	}
}

string Client::returnDate(int index, int cantTweets) {
	if (index >= 0 && index < cantTweets) {
		return alltweets[index].GetFecha();
	}
	else {
		return alltweets[0].GetFecha();
	}
}