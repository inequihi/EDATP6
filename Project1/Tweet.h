#pragma once
#include <string>
#define MAX_TWITTER_NAME 15

using namespace std;

class Tweet {

public:
	Tweet();
	Tweet(string fecha_,string data_);
	~Tweet();
	string GetData();
	size_t GetSize();
	string GetFecha();
	

private:
	string transformData(string);
	string transformFecha(string);
	string data;
	string fecha;

};
