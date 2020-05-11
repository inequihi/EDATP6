#include "Tweet.h"

Tweet::Tweet()
{
	data = "";
	fecha = "";
}
Tweet::Tweet(string fecha_,string data_) {


	fecha_ = transformFecha(fecha_);
	data_ = transformData(data_);
	data = data_;
	fecha = fecha_;
	checkData();
}

void Tweet::checkData() {

	boost::replace_all(data,"�","a");
	boost::replace_all(data,"�", "e");
	boost::replace_all(data,"�", "i");
	boost::replace_all(data,"�", "o");
	boost::replace_all(data,"�", "u");

	boost::replace_all(data, "�", "A");
	boost::replace_all(data, "�", "E");
	boost::replace_all(data, "�", "I");
	boost::replace_all(data, "�", "O");
	boost::replace_all(data, "�", "U");

	boost::replace_all(data,"�", "n");
	boost::replace_all(data,"�", "N");

	boost::replace_all(data,"�", "");
	boost::replace_all(data,"�", "");

}


string Tweet::transformFecha(string fecha_)
{
	string dia, mes, a�o, hora;
	dia.assign(fecha_, 8, 2);
	mes.assign(fecha_, 4, 3);
	hora.assign(fecha_, 11, 5);
	a�o.assign(fecha_, 26, 4);
	fecha_ = dia + "/" + mes + "/" + a�o + " - " + hora;	
	
	return fecha_;
}

string Tweet::transformData(string data_)
{
	//Eliminamos el URL al final para mostrar
	int extended = data_.find("https");
	data_ = data_.substr(0, extended);
	//	data_.append("...");
	return data_;
}

Tweet::~Tweet()			//CREO Q NO HACE FALTA USARLO EN client.cpp LINEA 165
{
	
}

//GETTERS
string Tweet::GetData() {

	return data;

}
string Tweet::GetFecha() {

	return fecha;

}

size_t Tweet::GetSize() {

	return data.size();

}

