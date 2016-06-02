#include <iostream>
#include <climits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <curl/curl.h>
#include <grove.hpp>
#include <ctime>

using namespace std;


const char* URL = "http://XXX.XXX.XX.XX:3000/sendData";//your url and port
const float LAT = 00.00; //latitude
const float LNG = 00.00; //longitude
const string CITY_NAME = "YOUR_CITY";


//get the current date and return a string
string getDate (){
	 time_t rawtime;
	 struct tm * timeinfo;
	 char buffer[80];

	 time (&rawtime);
	 timeinfo = localtime(&rawtime);

	 strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
	 std::string str(buffer);

	 std::cout << str;

	 return str;

}


void sendData(float temperature, float luminosity){

	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, URL);

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "postman-token: 882a1714-f65c-bd28-66fb-07344a1f7ee0");
	headers = curl_slist_append(headers, "cache-control: no-cache");
	headers = curl_slist_append(headers, "content-type: application/json");

	std::ostringstream dataInput;
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

	dataInput << "{\"light\":";
	dataInput << luminosity << ",\n";
	dataInput <<"\"temperature\":"<< temperature <<",\n";
	dataInput << "\"deviceName\":\""<< CITY_NAME <<"\",\n";
	dataInput << "\"lat\":" << LAT << ",\n";
	dataInput << "\"lng\":"<< LNG << ",\n";
	dataInput << "\"timeInstant\":\""<<getDate()<<"\"\n}";

	string outPutstr = dataInput.str();

	std::cerr << outPutstr;
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS,outPutstr.c_str());


	CURLcode ret = curl_easy_perform(hnd);

	if (ret == CURLE_OK ){
		std::clog << "Data uploaded"<< std::endl;
	}else{
		std::clog << "Error --Check your data"<< std::endl;

	}


}

int main()
{
	// check that we are running on Galileo or Edison
	mraa_platform_t platform = mraa_get_platform_type();
	if ((platform != MRAA_INTEL_GALILEO_GEN1) &&
			(platform != MRAA_INTEL_GALILEO_GEN2) &&
			(platform != MRAA_INTEL_EDISON_FAB_C)) {
		std::cerr << "Unsupported platform, exiting" << std::endl;
		return MRAA_ERROR_INVALID_PLATFORM;
	}

	upm::GroveTemp *temperatureSensor = new upm::GroveTemp(1);
	upm::GroveLight *luminositySensor = new upm::GroveLight(0);

	// simple error checking
	if ((temperatureSensor == NULL) || (luminositySensor == NULL) ) {
			std::cerr << "Can't create all objects, exiting" << std::endl;
			return MRAA_ERROR_UNSPECIFIED;
		}

	// loop forever updating the temperature values every second
	for (;;) {
		//read Data using lib upm
		float temp = temperatureSensor->value();
		float lux = luminositySensor->value();
		sendData(temp,lux);
		sleep(20);

	}

	return MRAA_SUCCESS;
}




