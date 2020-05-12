#pragma once

#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"
#include <string>
#include"Tweet.h"
#include <vector>
#include <memory>
#include "boost/locale.hpp"

//#include<boost/locale.hpp>

#include <memory>

using json = nlohmann::json;
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);



class Client {

public:
	Client();
	Client(std::string usuario_,int nTweets_);
	
	bool GetTweets();
	bool GetToken();
	string returnTweet(int index, int cantTweets);
	string returnDate(int index, int cantTweets);
	

private:
	void printTweets(std::vector<Tweet> tweets_);
	json Jdata;
	CURL* curl;					//Variable donde vamos a guardar las configuraciones de una transferencia
	CURLM* multiHandle;			//Variable donde vamos a atachear los easy handles
	CURLcode codErr;
	std::string API_key = "HCB39Q15wIoH61KIkY5faRDf6";
	std::string API_SecretKey = "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6";
	std::string usuario = "";  
	std::string myData = "";
	std::string query = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=";
	unsigned int numTweets = 0;
	//std::vector<Tweet> tweets;
	std::list<std::string> names;
	std::string readString, token;
	std::vector<Tweet> alltweets; 
	int stillRunning;

};