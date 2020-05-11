#pragma once
#include <string>
#include  <boost/algorithm/string/replace.hpp>
#define MAX_TWITTER_NAME 15

using namespace std;

class Tweet {

public:
	Tweet();
	Tweet(string fecha_, string data_, string usuario_);
	~Tweet();
	string GetData();
	size_t GetSize();
	string GetFecha();
	void checkData();
	unsigned int GetPos() { return pos; }
	void SetPos(unsigned int newpos) { pos = newpos; }


private:
	string transformData(string);
	string transformFecha(string);
	string data;
	string fecha;
	unsigned int pos;

};
