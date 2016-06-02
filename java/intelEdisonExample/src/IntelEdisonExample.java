import java.io.IOException;

import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;
import upm_grove.GroveLight;
import upm_grove.GroveTemp;
import mraa.Platform;
import mraa.mraa;

public class IntelEdisonExample {

private static String CITY = "LEÓN";
private static float LAT = 39.988641f;
private static float LNG = -3.933538f;

 public static void main(String[] args) {
    // TODO Auto-generated method stub
	  

	  Platform platform = mraa.getPlatformType();
	  
	  //check the platform type
      if (platform != Platform.INTEL_GALILEO_GEN1 &&
              platform != Platform.INTEL_GALILEO_GEN2 &&
              platform != Platform.INTEL_EDISON_FAB_C) {
          System.err.println("Unsupported platform, exiting");
          return;
      }
      
      //Declare  variables of the sensor using lib upm
      GroveTemp temperatureSensor = new GroveTemp(1);
      GroveLight luminositySensor = new GroveLight(0);
      
      while (true) {
    	//Read sensors values
    	int temp = temperatureSensor.value();
    	int lux = luminositySensor.value();
    	
    	System.out.println("Temperature: "+ String.valueOf(temp));
    	System.out.println("Luminosity: "+ String.valueOf(lux));
    	
    	try {
    		
			sendData(temp,lux);
			
		} catch (IOException e) {
			
			e.printStackTrace();
		}
    	
    	try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	
      }
	  
	
  }
 
 /***
  * Send data to the service to upload the city params 
  * @param temperature
  * @param iluminance
  * @throws IOException
  */
  private static void sendData(float temperature, float iluminance) throws IOException{
	  OkHttpClient client = new OkHttpClient();

	  MediaType mediaType = MediaType.parse("application/json");
	  RequestBody body = RequestBody.create(mediaType, "{\n    \"light\":"+iluminance+
			  ",\n    \"temperature\":"+temperature+
			  ",\n    \"deviceName\":"+"\""+CITY+"\""+
			  ",\n    \"lat\":"+LAT+
			  ",\n    \"lng\": "+LNG+
			  ",\n    \"timeInstant\":"+"\""+"21/04/2016 16:00:45"+"\"\n}");

	  Request request = new Request.Builder()
	    .url("http://192.168.90.25:3000/sendData")
	    .post(body)
	    .addHeader("content-type", "application/json")
	    .build();
	  
	  System.out.println(body.toString());
	  Response response = client.newCall(request).execute();
	  
		if (response.isSuccessful()){
			System.out.println("Data Uploaded to example server");
		}else{
			System.out.println(String.format("Error Status Code:%s",response.message()));
		}	 
		
  }
  
  

}