/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */



var CITY_NAME = 'your-city-name';
var LAT = '00.00';
var LNG = '00.00';
var ENDPOINT = 'xxxx.xx.xx.xxxx';
var URL = 'http://'+ENDPOINT +':3000/sendData';

var mraa = require('mraa'); //require mraa
console.log('MRAA Version: ' + mraa.getVersion()); //write the mraa version to the console

var groveSensor = require('jsupm_grove');

//Read data from sensors using libupm
var temp = new groveSensor.GroveTemp(0);
var light = new groveSensor.GroveLight(0);

function startSensorWatch() {
    setInterval(function () {
		
		//read the data from the sensorR
		var celsius_temperature = temp.value();
		var lux = light.value();
		sendData(celsius_temperature,lux);
		
    }, 60000);
}


startSensorWatch();

function sendData(temperature,luminosity){
	
	var request = require("request");
	
	var date = new Date().toISOString().replace(/T/, ' ').replace(/\..+/, '');

	var options = { method: 'POST',
	url: URL,
	headers: 
	   { 'postman-token': '8052c833-b33f-29b7-b995-aa5408212b89',
		 'cache-control': 'no-cache',
		 'content-type': 'application/json' },
	body: 
	   { light: luminosity,
		 temperature: temperature,
		 deviceName: CITY_NAME,
		 lat: LAT,
		 lng:LNG,
		 timeInstant: date },
	json: true };

	request(options, function (error, response, body) {
		if (error){
		console.log('--ERROR connectivity problems');	
		}else{
			if (response.statusCode == STATUS_CODE_OK){
				console.log('--Data uploaded');
			}else{
				console.log('--ERROR Status Code: '+response.statusCode);
			}
	}
	});
}