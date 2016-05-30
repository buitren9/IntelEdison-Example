  #include <SPI.h>
  #include <WiFi.h>


  //################# WIFI DETAILS #################
  char ssid[] = "";     // your network SSID (name)
  char pass[] = "";    // your network password (use for WPA, or use as key for WEP)
  int keyIndex = 0;            // your network key Index number (needed only for WEP)
  int status = WL_IDLE_STATUS;
  WiFiClient client;

//################ UEM DEMO VARIABLES ################

  char SERVER[] = "xxx.xx.xx.xxx";//your server address
  int PORT = yyyy; //port
  char city[] = "YOUR_CITY"; //name of your city
  char latitud[] = "YY.zzzz"; // latitude of the city
  char longitud[] = "YY.zzzz"; //longitude of the city

  //################ SENSOR VARIABLES ################
  int lum = 0;
  int temp = 0;
  float temperature;
  int B=3975;
  float resistance;

  // Wait 10s between measures
  int MEASURES_PERIOD = 100000;


  float readLuminosityFromPin (int pin){
      int val =  analogRead(pin);
      float Vout=val*0.0048828125;
      return(2500/(10*((5-Vout)/Vout)));
  }

  float readTemperatureFromPin(int pin){

    float aValue = analogRead(pin);                               // get analog value
    int B = 3975;
    float resistance = (1023 - aValue) * 10000 / aValue;
    float celsiusTmp = 1 / (log(resistance / 10000) / B + 1 / 298.15 ) - 273.15;
    return celsiusTmp;
  }

  void setup()
  {
    // Put your setup code here, to run once:
    // initialize serial communications at 115200 bps:
    Serial.begin(115200);
    pinMode(6, OUTPUT);

    setupNetwork();
  }


  void loop()
  {
    // Read sensors values on analogic PINs and send measures
    Serial.print("Starting the loop ...");
    readSensors();
    postMeasures(lum, temp);
    delay(MEASURES_PERIOD);

  }


  void setupNetwork()
  {
    Serial.print("Configuring Network...");

    // Check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      // don't continue:
      while(true);
    }

    String fv = WiFi.firmwareVersion();
    if( fv != "1.1.0" )
      Serial.println("Please upgrade the firmware");

    // Attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);
      //status = WiFi.begin(ssid);
      // Wait 10 seconds for connection:
      delay(10000);
    }
    Serial.println("Connected to wifi");
  }


  void readSensors()
  {
    // Reading Light and Luminosity Sensors contained in Grove Starter Kit for Arduino

    // Connect Light Sensor on Analogic PIN A0
    Serial.println("Reading from sensonrs");

    lum = readLuminosityFromPin(A0);

    // Connect TEMP Sensor on Analogic PIN A1
    temp = readTemperatureFromPin(A0);
    Serial.println("Luminosity "+ String(lum));
    Serial.println("Temperature "+ String(temp));
  }

  void postMeasures(int lum, int temp)
  {
    Serial.println("\nStarting connection to server...");

    String body = "{\"light\":";
    body += lum;
    body += ",\"temperature\":";
    body += temp;
    body += ",\"deviceName\":\"";
    body += city;
    body+="\",\"lat\":";
    body += latitud;
    body += ",\"lng\":";
    body += longitud;
    body += ",\"timeInstant\":\"25/05/2016 20:34:45\"}";

    // If you get a connection, report back via serial:
    if (client.connect(SERVER, PORT)) {
      Serial.println("Connected to server");

    // Make a HTTP request:
    client.println("POST /SendData HTTP/1.1");
    client.println("Host:"+String(SERVER)+":"+String(PORT));
    client.println("Content-Type: application/json");
    client.println("Content-Length: "+String(body.length()));
    client.println("Connection: close");
    client.println();
    client.println(body);

    //Checking the response
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }

    Serial.println ("");
    client.stop();
    Serial.println(body);
    Serial.println("Disconnected from server");
    }
  }
