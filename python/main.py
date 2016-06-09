# -*- coding: utf-8 -*-
import pyupm_grove as grove
import mraa
import requests
import time
import datetime

from mySensors import MySensors

#Time between measures
MEASURES_PERIOD = 200
CITY_NAME = "YOUR_CITY_NAME"
HOST = "xxx.xxx.xx.xx"
LAT = "0.00"
LNG = "0.00"

temperature = None
luminosity = None

#Send temperature and luminosity to a server to upload all data in cartoDB map
def sendMeasures(temperature,luminosity):

    url = "http://"+HOST+":3000/sendData"
    payload = "{\n\"light\":"+str(luminosity)+",\n"
    payload = payload + "\"temperature\":"+str(temperature)+",\n"
    payload = payload + "\"deviceName\":\""+CITY_NAME+"\",\n"
    payload = payload + "\"lat\":"+LAT+",\n"
    payload = payload + "\"lng\":"+LNG+",\n"
    payload = payload + "\"timeInstant\":\""+str(datetime.datetime.now().time())+ time.strftime(" %d/%m/%Y")+"\"\n}"

    print str(payload)
    headers = {
        'content-type': "application/json"
        }
    try:
        r = requests.request("POST", url, data=payload, headers=headers)
        print "Response Status Code: "+str(r.status_code)
    except:
        print "Error chek your URL or Connection, please"

#Read measures from sensor using libupm
def readMeasures():

    tempSensor =  grove.GroveTemp(0)
    lightSensor = grove.GroveLight(0)
    global temperature
    global luminosity
    temperature = tempSensor.value()
    luminosity = lightSensor.value()

while True:

    readMeasures()
    sendMeasures(temperature,luminosity)
    time.sleep(MEASURES_PERIOD)
