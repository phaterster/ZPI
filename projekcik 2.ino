



/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>



// Replace with your network details
const char* ssid = "UPC57883A3";
const char* password = "d2zBbcrxfpjb";

//const char* ssid = "NT-23e";
//const char* password = "siecwsizbb";

// Data wire is plugged into pin D1 on the ESP8266 12-E - GPIO 5
#define ONE_WIRE_BUS D3
int awers = D0;
int rewers = D1;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

unsigned long startMillis;  
unsigned long currentMillis;
int fase;


const unsigned long period = 1000; 

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature DS18B20(&oneWire);
char temperatureCString[7];
char temperatureCString2[7];
char temperatureCString3[7];
char temperatureCString4[7];
// Web Server on port 80
WiFiServer server(80);

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);


  
   fase = 0;
  startMillis = millis(); 

   
  DS18B20.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  pinMode(awers, OUTPUT); 
  pinMode(rewers, OUTPUT); 
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

void getTemperature() {
  float tempC;
  float tempC3;
  float tempC2;
  float tempC4;
  do {
    DS18B20.requestTemperatures(); 
    tempC = DS18B20.getTempCByIndex(0);
    dtostrf(tempC, 2, 2, temperatureCString);
    tempC3 = DS18B20.getTempFByIndex(1);
    dtostrf(tempC3, 3, 2, temperatureCString3);
    tempC2 = DS18B20.getTempCByIndex(2);
    dtostrf(tempC2, 4, 2, temperatureCString2);
       tempC4 = DS18B20.getTempCByIndex(3);
    dtostrf(tempC4, 5, 2, temperatureCString4);
    delay(100);
  } while (tempC == 85.0 || tempC == (-127.0));
}

// runs over and over again
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();

      currentMillis = millis();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();


        
///////


if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http,http2;  //Object of class HTTPClient
    //http.begin("http://10.10.23.162:8445/status");
    http.begin("http://192.168.0.115:8445/status");
    http2.begin("http://192.168.0.115:8445/freq");
    int httpCode = http.GET();
    int httpCode2 = http2.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      JsonObject& root2 = jsonBuffer.parseObject(http2.getString());
      // Parameters
      
      const char* direction2 = root["direction"]; // "Bret"
      const char* runningState = root["runningState"]; // "Sincere@april.biz"
      const char* nr = root2["frequency"];
      // Output to serial monitor
    //  Serial.print("RunningState:");
     // Serial.println(runningState);
     // Serial.print("Direction:");
     // Serial.println(direction2);
     // if(strcmp(runningState,"1")==0){

      //  digitalWrite(awers, HIGH);
        //Serial.print("awers");
      //  digitalWrite(rewers, LOW);
       // }


       //if(strcmp(runningState,"-1")==0){
       // digitalWrite(rewers, HIGH);
       // digitalWrite(awers, LOW);
        //Serial.print("rewers");
       // }

unsigned long number = atol(nr);
// Serial.print(number);


 if(strcmp(runningState,"1")==0){

      if (currentMillis - startMillis >= number)  //test whether the period has elapsed
  {
    if (fase == 0)
    {
      digitalWrite(awers, LOW);
      startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
      fase = 1; //increment fase, so next action will be different
    }
    else if (fase == 1)
    {               
      digitalWrite(rewers, HIGH);  
      startMillis = currentMillis; 
      fase = 2;
    }
    else if (fase == 2)
    {
      digitalWrite(rewers, LOW);
      startMillis = currentMillis; 
      fase = 3;
    }
    else if (fase == 3)
    {
      digitalWrite(awers, HIGH);
      fase = 0;
      startMillis = currentMillis; 
    }
  }
       
        }
        

        if(strcmp(runningState,"0")==0){

        digitalWrite(awers, LOW);
        digitalWrite(rewers, LOW);
       
        }



    


     
     
        
         }

    
    http.end();   //Close connection




/*HTTPClient http2;  //Object of class HTTPClient
    //http.begin("http://10.10.23.162:8445/status");
    http2.begin("http://192.168.0.115:8445/freq");
    int httpCode2 = http2.GET();
    //Check the returning code                                                                  
    if (httpCode2 > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http2.getString());

    const char* nr = root["frequency"];
    //int nr(const char * line1 = root["frequency"]);
    //const char* nr = root[0]["frequency"];
     //long number = atol(nr);
   // Serial.println(nr);
  //  delay(number);
    
   


    }


    http.end();

*/
 
  
      //Send a request every 30 seconds

          
}


//////


        
        
        if (c == '\n' && blank_line) {
            getTemperature();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature
            client.print("[");
            client.print(temperatureCString);
             client.print(",");
             client.print(temperatureCString2);
             client.print(",");
             client.print(temperatureCString3);
             client.print(",");
             client.print(temperatureCString4);
             client.print("]");
            
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }

  
}   


 
  
 
