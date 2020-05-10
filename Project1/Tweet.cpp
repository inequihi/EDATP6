#include "Tweet.h"

Tweet::Tweet(string fecha_,string data_) {

	data = data_;
	fecha = fecha_;
}

//GETTERS
string Tweet::GetData() {

	return data;

}
string Tweet::GetFecha() {

	return fecha;

}

int Tweet::GetSize() {

	return data.size();

}

