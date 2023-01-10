/*
  many, many examples were combined to do this

  https://circuits4you.com/2018/11/21/esp32-access-point-station-ap-html-web-server-arduino/
   was particularily informative
  many thanks go to Rui Santos and his crew at RandomNerdTutorials.com
  https://gabrielromualdo.com/articles/2020-12-15-how-to-use-the-html5-gamepad-api was particularily helpful in the bluetooth area.

*/

// Head lights stuff

#include "MeccaBrain.h"

const int chainPin1 = 23;     // Pin for the head lights
MeccaBrain chain1(chainPin1);

int PWM_Speed = 250; //  speed PWM duty cycle   

// MAYBE YOU NEED TO CHANGE THIS intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment
int pos = 90;
      
#define servoPin 15 //  Speed

#define MOTORRight_PWM 26 //  Speed
#define MOTORRight_DIR 25 //  Direction
#define MOTORLeft_PWM 32 //  Speed
#define MOTORLeft_DIR 33 //  Direction

#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0

const int freq = 5000;
const int ledChannelLeft_PWM = 10;
const int ledChannelLeft_DIR = 11;
const int ledChannelRight_PWM = 12;
const int ledChannelRight_DIR = 13;
const int resolution = 8;

#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  //Web page header file
#include <Servo.h>
Servo myservo;   

const char* ssid = "Your PLaces";               // different name and password?
const char* password = "********";                    // password length minimum 8 char

WebServer server(80);

void setup(void) {

  Serial.begin(115200);
  pinMode(chainPin1, OUTPUT);
//  Serial.println("kilroy");
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(pos);

  ledcSetup(ledChannelLeft_PWM, freq, resolution);
  ledcSetup(ledChannelLeft_DIR, freq, resolution);
  ledcSetup(ledChannelRight_PWM, freq, resolution);
  ledcSetup(ledChannelRight_DIR, freq, resolution);
  
  pinMode( MOTORLeft_DIR, OUTPUT );
  pinMode( MOTORLeft_PWM, OUTPUT );
  pinMode( MOTORRight_DIR, OUTPUT );
  pinMode( MOTORRight_PWM, OUTPUT );
  
  ledcAttachPin(MOTORLeft_PWM, ledChannelLeft_PWM);
  ledcAttachPin(MOTORLeft_DIR, ledChannelLeft_DIR);
  ledcAttachPin(MOTORRight_PWM, ledChannelRight_PWM);
  ledcAttachPin(MOTORRight_DIR, ledChannelRight_DIR);
  
  ledcWrite( ledChannelLeft_DIR, dir_Forward );
  ledcWrite( ledChannelLeft_PWM, PWM_STOP );
  ledcWrite( ledChannelRight_DIR, dir_Forward );
  ledcWrite( ledChannelRight_PWM, PWM_STOP );
  
  Serial.println();
  Serial.println("Booting Sketch...");

   
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();


 
  server.on("/", handle_OnConnect);                      //  commands are sent from html page index.h
  server.on("/A", handle_Go_Lefter);                  //  what they do is described below
  server.on("/B", handle_Forward);
  server.on("/C", handle_Go_Righter);
  server.on("/G", handle_Back);
  server.on("/I", handle_Spin_Left);
  server.on("/J", handle_Spin_Right);
  server.on("/off", handle_Stop);
  server.on("/FO", handle_Flashlight_Off);
  server.on("/Flon", handle_Flashlight_On);
  server.onNotFound(handle_NotFound);

  server.begin();                  //Start server

}


void loop(void) {
  server.handleClient();
}


void handle_NotFound () {
}


void handle_OnConnect() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page

}


void handle_Forward() {
 
  ledcWrite( ledChannelLeft_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLeft_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRight_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRight_PWM, PWM_Speed ); // PWM speed = fast
  server.send(200, "text/plain", "ok");
}


void handle_Back() {
 
  ledcWrite( ledChannelLeft_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelLeft_PWM, 255 - PWM_Speed ); // PWM speed = Slow  ledChannel_PWM, 180 - PWM_Speed
  ledcWrite( ledChannelRight_DIR, dir_Back  ); 
  ledcWrite( ledChannelRight_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {

  ledcWrite( ledChannelLeft_DIR, dir_Forward);
  ledcWrite( ledChannelLeft_PWM, PWM_STOP );
  ledcWrite( ledChannelRight_DIR, dir_Forward ); 
  ledcWrite( ledChannelRight_PWM, PWM_STOP );  
  server.send(200, "text/plain", "ok");
}

void handle_Spin_Left() {
  ledcWrite( ledChannelRight_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelRight_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelLeft_DIR, dir_Back ); 
  ledcWrite( ledChannelLeft_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}

void handle_Spin_Right() {
  ledcWrite( ledChannelLeft_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelLeft_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRight_DIR, dir_Back ); 
  ledcWrite( ledChannelRight_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}

void handle_Go_Righter() { // Receive an HTTP GET request for Head Movement

  pos = pos - 20;
  if (pos < 20) {
    pos = 20;
  }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}

void handle_Go_Lefter() { // Receive an HTTP GET request for Head Movement
 
  pos = pos + 20;
  if (pos > 250) {
    pos = 250;
  }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}

void handle_Flashlight_On() {

  chain1.setLEDColor(7, 7, 7, 0);
  int j=0;
  while(j<10){
  chain1.communicate();
  j++;
  }

  server.send(200, "text/plain", "ok");
}

void handle_Flashlight_Off() {

  chain1.setLEDColor(0, 0, 0, 0);
  int i=0;
  while(i<10){
    chain1.communicate();
  i++;
  }

  server.send(200, "text/plain", "ok");

}
