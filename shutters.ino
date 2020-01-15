#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h> 

ESP8266WebServer server(80);

//network settings
const char* ssid = "NETWORK ID";
const char* password = "NETWORK PASSWORD";

//available pins
int pins[] = {14,12,13,15};

//lets start from 0
int curr_pin = 0;


//direction 0 -> close, 1 -> open
int dir = 0;

//current loops
int loops = 0;


//wrong request
void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); 
};


void setup() {
  //set output for pins
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  //connect with wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print("Connecting...");
  }

  //configure routes
  server.on("/open", startOpen);  
  server.on("/close", startClose);
  //404
  server.onNotFound(handleNotFound);

  //start
  server.begin();
}

//init opening
void startOpen(){
  if(loops > 0|| dir == 1)return;
  loops = 10000;
  dir = 1;
}
//init closing
void startClose(){
  if(loops > 0 || dir == 0)return;
  loops = 10000;
  dir = 0;
}

void loop() {
  //check for http
 server.handleClient();
 //while number of loops is less bigger than 0 handle current state (closing/opening)
 if(loops > 0){
  if(dir == 0)closeFunction();
  if(dir == 1)openFunction();
 }
 delay(2);
}

//iterate through every pin for the motor and handle it, goes from pin 1 to pin 4 to init rotation
void openFunction(){
  curr_pin = (curr_pin > 3 ? 0 : curr_pin + 1);
  for(int x = 0; x < 4; x++){
      digitalWrite(pins[x], (x == curr_pin ? HIGH : LOW));
  }
  loops--;
}
void closeFunction(){
  curr_pin = (curr_pin == 0 ? 3 : curr_pin - 1);
  for(int x = 3; x >= 0; x--){
      digitalWrite(pins[x], (x == curr_pin ? HIGH : LOW));
  }
  loops--;
}
