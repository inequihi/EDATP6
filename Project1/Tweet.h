#pragma once
#include <string>

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
