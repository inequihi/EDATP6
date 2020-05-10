#pragma once
#include <string>

using namespace std;

class Tweet {

public:

	Tweet(string fecha_,string data_);

	string GetData();
	int GetSize();
	string GetFecha();
	

private:

	string data;
	string fecha;

};
