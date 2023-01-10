 /*
   Adapted from the following:
   
   ESP32 Web Server Demo using Accesspoint
   https://circuits4you.com
   21-11-2018

   Many thanks to Rui Santos and his crew over at randomnerdturtorials.com
   https://gabrielromualdo.com/articles/2020-12-15-how-to-use-the-html5-gamepad-api was very helpful in the bluetooth area.
   
*/
// Head lights stuff

#include "MeccaBrain.h"

const int chainPin1 = 26;     // Pin for the head lights
MeccaBrain chain1(chainPin1);
int hpos = 90;    // Head position

#include <Servo.h>   //  servo stuff
Servo myservo;  // create servo object to control a servo
Servo myHeadservo;
// GPIO the servo is attached to
static const int servoPin = 15;      
static const int hservoPin = 27; 

int pos = 100; // intitial servo postion to be at 90 degrees to make it straight different from 90 for mechanical adjustment
// wired connections
int MOTOR_PWM_A = 18;//  Speed  Enable Pin
int MOTOR_DIR_A = 5; //  Direction
int MOTOR_DIR1_A = 19;//  Direction
int MOTOR_PWM_B = 23 ;//  Speed  Enable Pin
int MOTOR_DIR_B = 22; //  Direction
int MOTOR_DIR1_B = 21;//  Direction

// the actual values for "fast" and "slow" depend on the motor
#define dir_Forward 0
#define dir_Back 255

#define PWM_STOP 0
#define PWM_SLOW 100 // arbitrary slow speed PWM duty cycle
#define PWM_FAST 255 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

const int freq = 909;  // 1.1ms per bit
const int resolution = 8;
const int ledChannel_PWM_A = 10;
const int ledChannel_PWM_B = 11;

#include <WiFi.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

//Enter your desired WiFi SSID and PASSWORD
const char* ssid = "ESP_Rider_Big_Chassis";
const char* password = "********";  // change the *'s to your favourite vegetable

WebServer server(80);

//===============================================================
// Setup
//===============================================================
void setup(void) {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin); 
  myHeadservo.attach(hservoPin);
  myservo.write(pos);
  myHeadservo.write(hpos);
  Serial.println(pos);


  pinMode(MOTOR_PWM_A, OUTPUT);
  pinMode(MOTOR_DIR_A, OUTPUT);
  pinMode(MOTOR_DIR1_A, OUTPUT);
  pinMode(MOTOR_PWM_B, OUTPUT);
  pinMode(MOTOR_DIR_B, OUTPUT);
  pinMode(MOTOR_DIR1_B, OUTPUT);

  ledcSetup(ledChannel_PWM_A, freq, resolution);
  ledcSetup(ledChannel_PWM_B, freq, resolution);
 
  ledcAttachPin(MOTOR_PWM_A, ledChannel_PWM_A);
  ledcAttachPin(MOTOR_PWM_B, ledChannel_PWM_B);


  ledcWrite( ledChannel_PWM_A, PWM_STOP );
  ledcWrite( ledChannel_PWM_B, PWM_STOP );

  //ESP32 As access point IP: 192.168.4.1
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);    //Password length minimum 8 char

 
  server.on("/", handle_OnConnect);
  server.on("/A", handle_Forward_Fast);
  server.on("/B", handle_Forward_Slow);
  server.on("/C", handle_Back_Slow);
  server.on("/D", handle_Back_Fast);
  server.on("/off", handle_Stop);
  server.on("/R", handle_Go_Righter);
  server.on("/L", handle_Go_Lefter);
  server.on("/HR", handle_Head_Righter);
  server.on("/HL", handle_Head_Lefter);
  server.on("/FO", handle_Head_light_Off);
  server.on("/Flon", handle_Head_light_On);
  server.on("/Pos", handle_Steering_Slider_Input);
  server.onNotFound(handle_NotFound);


  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void) {
  server.handleClient();
}
void handle_NotFound () {
}


void handle_OnConnect() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page

}

void handle_Steering_Slider_Input() {
//  server.send(200, "text/plain", "ok");
  if(server.arg("value") ){
     int slidervalue = server.arg("value").toInt();   //
    Serial.print("slidervalue = ");
   // myservo.write(slidervalue);
     Serial.println(slidervalue);
     pos = slidervalue;
    myservo.write(pos);
     }
  Serial.print("pos = ");
  Serial.print(pos);
  server.send(200, "text/plain", "ok");
}

void handle_Forward_Fast() {
  ledcWrite( ledChannel_PWM_A, PWM_FAST ); // PWM speed = fast
  ledcWrite( ledChannel_PWM_B, PWM_FAST ); // PWM speed = fast
  digitalWrite(MOTOR_DIR_A, HIGH);
  digitalWrite(MOTOR_DIR1_A, LOW); 
  digitalWrite(MOTOR_DIR_B, LOW);
  digitalWrite(MOTOR_DIR1_B, HIGH); 
  server.send(200, "text/plain", "ok");
}

void handle_Forward_Slow() {
  ledcWrite( ledChannel_PWM_A, PWM_SLOW  ); 
  ledcWrite( ledChannel_PWM_B, PWM_SLOW  ); 
  digitalWrite(MOTOR_DIR_A,HIGH );
  digitalWrite(MOTOR_DIR1_A, LOW); 
  digitalWrite(MOTOR_DIR_B, LOW);
  digitalWrite(MOTOR_DIR1_B, HIGH); 
  server.send(200, "text/plain", "ok");
}

void handle_Back_Slow() {
  ledcWrite( ledChannel_PWM_A, PWM_SLOW ); 
  ledcWrite( ledChannel_PWM_B, PWM_SLOW ); 
  digitalWrite(MOTOR_DIR_A, LOW);
  digitalWrite(MOTOR_DIR1_A, HIGH); 
  digitalWrite(MOTOR_DIR_B, HIGH);
  digitalWrite(MOTOR_DIR1_B, LOW); 
  server.send(200, "text/plain", "ok");
}

void handle_Back_Fast() {
  ledcWrite( ledChannel_PWM_A, PWM_FAST ); 
  ledcWrite( ledChannel_PWM_B, PWM_FAST ); 
  digitalWrite(MOTOR_DIR_A, LOW);
  digitalWrite(MOTOR_DIR1_A, HIGH); 
  digitalWrite(MOTOR_DIR_B, HIGH);
  digitalWrite(MOTOR_DIR1_B, LOW); 
  server.send(200, "text/plain", "ok");
}

void handle_Stop() {
  ledcWrite( ledChannel_PWM_A, PWM_STOP );
  ledcWrite( ledChannel_PWM_B, PWM_STOP );
  digitalWrite(MOTOR_DIR_A, LOW);
  digitalWrite(MOTOR_DIR1_A, LOW); 
  digitalWrite(MOTOR_DIR_B, LOW);
  digitalWrite(MOTOR_DIR1_B, LOW); 
  server.send(200, "text/plain", "ok");
}

void handle_Go_Lefter() { // Receive an HTTP GET request for steering.  Depending on the position of sg90 servo could change name to Lefter
  pos = pos - 5;
  if (pos < 77) {  //  the 27 here and 153 below are dependant on the free swing room available, the slider bar in the index.h should be set the same
    pos = 77;
  }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}


void handle_Go_Righter() { // Receive an HTTP GET request for steering.  And from above comment this could be renamed Righter instead
  pos = pos + 5;
  if (pos > 123) {
    pos = 123;
  }
  myservo.write(pos);
  server.send(200, "text/plain", "ok");
}


void handle_Head_Righter() { // Receive an HTTP GET request for steering
  hpos = hpos - 15;
  if (hpos < 0) {
    hpos = 0;
  }
  myHeadservo.write(hpos+7);
  myHeadservo.write(hpos+4);
  myHeadservo.write(hpos+3);
  myHeadservo.write(hpos+2);
  myHeadservo.write(hpos+1);
  myHeadservo.write(hpos);
  server.send(200, "text/plain", "ok");
}


void handle_Head_Lefter() { // Receive an HTTP GET request for steering
  hpos = hpos + 15;
  if (hpos > 180) {
    hpos = 180;
  }
  myHeadservo.write(hpos-7);
  myHeadservo.write(hpos-4);
  myHeadservo.write(hpos-3);
  myHeadservo.write(hpos-2);
  myHeadservo.write(hpos-1);
  myHeadservo.write(hpos);
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

