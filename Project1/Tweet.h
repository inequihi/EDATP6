#pragma once
#include <string>
#include  <boost/algorithm/string/replace.hpp>
#define MAX_TWITTER_NAME 15
#define Atilde 160
#define Etilde 130
#define Itilde 161
#define Otilde 162
#define Utilde 163
#define enie 164
#define ENIE 165

using namespace std;

class Tweet {

public:
	Tweet();
	Tweet(string fecha_, string data_);
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
