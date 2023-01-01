/*
  many, many examples were combined to do this

  https://circuits4you.com/2018/11/21/esp32-access-point-station-ap-html-web-server-arduino/
   was particularily informative
    many thanks go to Rui Santos and his crew at RandomNerdTutorials.com

*/
#include "MeccaBrain.h"        // Only used for meccano headset lights, had to HACK the 5 bottom lines of the .cpp file, added values to the arrays, shrug.
const int chainPin1 = 23;     // Pin for the head lights
MeccaBrain chain1(chainPin1);  // head lights magic

const int servoPin = 15;       // Head Servo Pin

int PWM_Speed = 250; // initial speed PWM duty cycle                                
                     // MAYBE YOU NEED TO CHANGE THIS intitial Head servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment

int pos = 90;        // starting head position for servo 

                     // wired motor connections
#define MOTORLR_PWM 26 //  Speed
#define MOTORLR_DIR 25 //  Direction
#define MOTORRR_PWM 33 //  Speed
#define MOTORRR_DIR 32 //  Direction
#define MOTORLF_PWM 14 //  Speed
#define MOTORLF_DIR 27 //  Direction
#define MOTORRF_PWM 13 //  Speed
#define MOTORRF_DIR 12 //  Direction

                       // for the L9110s Motor Driver
#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0


const int freq = 5000;
const int ledChannelLR_PWM = 10;
const int ledChannelLR_DIR = 11;
const int ledChannelRR_PWM = 12;
const int ledChannelRR_DIR = 13;
const int ledChannelLF_PWM = 14;
const int ledChannelLF_DIR = 15;
const int ledChannelRF_PWM = 8;
const int ledChannelRF_DIR = 9;
const int resolution = 8;

                      // wifi section

#include <WiFi.h>
#include <WebServer.h>

                      // get the data from the html file and define the servo
#include "index.h"    //Web page header file
#include <Servo.h>
Servo myservo;                                 

                      //Enter your WiFi SSID and PASSWORD     <<*******************************************************

//const char* ssid = "ESP";               // different name and password?
//const char* password = "network1";                    // password length minimum 8 char

const char* ssid = "aliens";               // different name and password?
const char* password = "aaaaaaaa";                    // password length minimum 8 char

WebServer server(80);  // normal webserver port

void setup(void) {
  Serial.begin(115200);
  pinMode(chainPin1, OUTPUT);        // set up the pins and center the head
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(pos);



  ledcSetup(ledChannelLR_PWM, freq, resolution);        //  ESP32 requirement to use pwm on the pins
  ledcSetup(ledChannelLR_DIR, freq, resolution);
  ledcSetup(ledChannelRR_PWM, freq, resolution);
  ledcSetup(ledChannelRR_DIR, freq, resolution);
  ledcSetup(ledChannelLF_PWM, freq, resolution);
  ledcSetup(ledChannelLF_DIR, freq, resolution);
  ledcSetup(ledChannelRF_PWM, freq, resolution);
  ledcSetup(ledChannelRF_DIR, freq, resolution);

  pinMode( MOTORLR_DIR, OUTPUT );                            // setup the motors to the correct wired situation by changing these numbers not moving the wires
  pinMode( MOTORLR_PWM, OUTPUT );
  pinMode( MOTORRR_DIR, OUTPUT );
  pinMode( MOTORRR_PWM, OUTPUT );
  pinMode( MOTORLF_DIR, OUTPUT );
  pinMode( MOTORLF_PWM, OUTPUT );
  pinMode( MOTORRF_DIR, OUTPUT );
  pinMode( MOTORRF_PWM, OUTPUT );

  ledcAttachPin(MOTORLR_PWM, ledChannelLR_PWM);              // connect the channel to the pins
  ledcAttachPin(MOTORLR_DIR, ledChannelLR_DIR);
  ledcAttachPin(MOTORRR_PWM, ledChannelRR_PWM);
  ledcAttachPin(MOTORRR_DIR, ledChannelRR_DIR);
  ledcAttachPin(MOTORLF_PWM, ledChannelLF_PWM);
  ledcAttachPin(MOTORLF_DIR, ledChannelLF_DIR);
  ledcAttachPin(MOTORRF_PWM, ledChannelRF_PWM);
  ledcAttachPin(MOTORRF_DIR, ledChannelRF_DIR);
  
  ledcWrite( ledChannelLR_DIR, dir_Forward );               // stop all the motors
  ledcWrite( ledChannelLR_PWM, PWM_STOP );
  ledcWrite( ledChannelRR_DIR, dir_Forward );
  ledcWrite( ledChannelRR_PWM, PWM_STOP );
  ledcWrite( ledChannelLF_DIR, dir_Forward );
  ledcWrite( ledChannelLF_PWM, PWM_STOP );
  ledcWrite( ledChannelRF_DIR, dir_Forward );
  ledcWrite( ledChannelRF_PWM, PWM_STOP );

  Serial.println();
  Serial.println("Booting Sketch...");

  //   wifi mode and initiate the connection
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  // WiFi.setTxPower(WIFI_POWER_8_5dBm);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
      
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();                  //Start server
  Serial.println("HTTP server started");
 
  server.on("/", handle_OnConnect);                      //  commands are sent from html page index.h
  server.on("/A", handle_Go_Lefter);                     //  what they do is described below
  server.on("/B", handle_Forward);
  server.on("/C", handle_Go_Righter);
  server.on("/D", handle_Left);
  server.on("/E", handle_Right);
  server.on("/G", handle_Back);
  server.on("/I", handle_Spin_Left);
  server.on("/J", handle_Spin_Right);
  server.on("/off", handle_Stop);
    
  server.on("/FO", handle_Head_light_Off);
  server.on("/Flon", handle_Head_light_On);

  server.onNotFound(handle_NotFound);

}


void loop(void) {
  server.handleClient();

}
void handle_NotFound () {
}


void handle_OnConnect() {

  String s = MAIN_page; //Read HTML contents
  Serial.println("connected:");
  server.send(200, "text/html", s); //Send web page

}

void handle_Forward() {
  
  ledcWrite( ledChannelLR_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLR_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRR_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRR_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelLF_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLF_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRF_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRF_PWM, PWM_Speed ); // PWM speed = fast

  server.send(200, "text/plain", "ok");
}

void handle_NW() {

  ledcWrite( ledChannelLR_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLR_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRF_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRF_PWM, PWM_Speed ); // PWM speed = fast

  server.send(200, "text/plain", "ok");
}
void handle_NE() {

  ledcWrite( ledChannelLF_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelLF_PWM, PWM_Speed ); // PWM speed = fast
  ledcWrite( ledChannelRR_DIR, dir_Forward ); // direction = forward
  ledcWrite( ledChannelRR_PWM, PWM_Speed ); // PWM speed = fast

  server.send(200, "text/plain", "ok");
}

void handle_Back() {

  ledcWrite( ledChannelLR_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelLR_PWM, 255 - PWM_Speed ); // PWM speed = Slow  ledChannel_PWM, 180 - PWM_Speed
  ledcWrite( ledChannelRR_DIR, dir_Back  ); 
  ledcWrite( ledChannelRR_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Back ); 
  ledcWrite( ledChannelLF_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Back ); 
  ledcWrite( ledChannelRF_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}

void handle_SE() {

  ledcWrite( ledChannelRF_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelRF_PWM, 255 - PWM_Speed ); // PWM speed = Retro
  ledcWrite( ledChannelLR_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelLR_PWM, 255 - PWM_Speed ); // PWM speed = Retro
  server.send(200, "text/plain", "ok");
}
void handle_SW() {

  ledcWrite( ledChannelLF_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelLF_PWM, 255 - PWM_Speed ); // PWM speed = Retro
  ledcWrite( ledChannelRR_DIR, dir_Back ); // direction = Back
  ledcWrite( ledChannelRR_PWM, 255 - PWM_Speed ); // PWM speed = Retro
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {

  ledcWrite( ledChannelLR_DIR, dir_Forward);
  ledcWrite( ledChannelLR_PWM, PWM_STOP );
  ledcWrite( ledChannelRR_DIR, dir_Forward ); 
  ledcWrite( ledChannelRR_PWM, PWM_STOP );  
  ledcWrite( ledChannelLF_DIR, dir_Forward ); 
  ledcWrite( ledChannelLF_PWM, PWM_STOP );
  ledcWrite( ledChannelRF_DIR, dir_Forward ); 
  ledcWrite( ledChannelRF_PWM, PWM_STOP ); 
  server.send(200, "text/plain", "ok");
}

void handle_Right() { 
  ledcWrite( ledChannelLR_DIR, dir_Back ); // direction = forward or back
  ledcWrite( ledChannelLR_PWM, 255 - PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRR_DIR, dir_Forward ); 
  ledcWrite( ledChannelRR_PWM, PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Forward ); 
  ledcWrite( ledChannelLF_PWM, PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Back ); 
  ledcWrite( ledChannelRF_PWM, 255 - PWM_Speed ); 
  server.send(200, "text/plain", "ok");
}

void handle_Left() { 
  ledcWrite( ledChannelLR_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelLR_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRR_DIR, dir_Back ); 
  ledcWrite( ledChannelRR_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Back ); 
  ledcWrite( ledChannelLF_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Forward ); 
  ledcWrite( ledChannelRF_PWM, PWM_Speed ); 

  server.send(200, "text/plain", "ok");
}
void handle_Spin_Left() { 
  ledcWrite( ledChannelRR_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelRR_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelLR_DIR, dir_Back ); 
  ledcWrite( ledChannelLR_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Back ); 
  ledcWrite( ledChannelLF_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Forward ); 
  ledcWrite( ledChannelRF_PWM, PWM_Speed ); 

  server.send(200, "text/plain", "ok");
}
void handle_Spin_Right() { 
  ledcWrite( ledChannelLR_DIR, dir_Forward ); // direction = forward or back
  ledcWrite( ledChannelLR_PWM, PWM_Speed ); // PWM speed = fast or slow
  ledcWrite( ledChannelRR_DIR, dir_Back ); 
  ledcWrite( ledChannelRR_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelRF_DIR, dir_Back ); 
  ledcWrite( ledChannelRF_PWM, 255 - PWM_Speed ); 
  ledcWrite( ledChannelLF_DIR, dir_Forward ); 
  ledcWrite( ledChannelLF_PWM, PWM_Speed ); 

  server.send(200, "text/plain", "ok");
}
void handle_Head_light_On() {

  chain1.setLEDColor(7, 7, 7, 0);
  int j=0;
  while(j<10){
  chain1.communicate();
  j++;
  }

  server.send(200, "text/plain", "ok");
}

void handle_Head_light_Off() {

  chain1.setLEDColor(0, 0, 0, 0);
  int i=0;
  while(i<10){
    chain1.communicate();
  i++;
  }

  server.send(200, "text/plain", "ok");

}

void handle_Go_Righter() { // Receive an HTTP GET request for steering

  pos = pos - 15;
  if (pos < 0) {
    pos = 0;
  }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}

void handle_Go_Lefter() { // Receive an HTTP GET request for steering

  pos = pos + 15;
  if (pos > 180) {
    pos = 180;
  }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}
