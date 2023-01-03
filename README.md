# Perky_and_Rides
Code for Perky's robot head's transportation options.  Tracked, Mecanum and Wheeled vehicles.

Perky consists of a StereoPi camera mounted on a Meccanoid headset.  The headset can be turned left and right and has lights that can be turned on and off.

Bluetooth control of a L9110S motor driver equipped tracked vehicle and another meccanum wheeled vehicle as well.
Bluetooth control of a L298N motor driver equipped 2 motor wheeled Road Warrior. ;)

Reguires a handheld BT VR Chip game controller. The one with android/ios switch on the side.

This software creates wepages on your local wifi network to control the vehicles using the joystick and buttons on the VR Chip
controller in various ways.

The Road Warrior uses an Access Point at 192.168.4.1

The controller is linked to your android phone in the usual bluetooth manner and then the webpage from the vehicle is loaded in your browser.
I use an Android app called Fing to find the ip addresses of the Stereopi and vehicle.  One cell phone is required for the bluetooth connection.
The web page of the vehicle will be blank or ask for a BT controller to be connected.  

The Stereopi sends a 3d view of the surroundings and the servo on the head allows for scanning around. 
Another cellphone will be dedicated to viewing the VR.  Android phones work.  Apple phones, shrug, not so much.  

Flash to your ESP32 with the Arduino IDE.

Pick from Tracked Vehicles using two motors, or meccanum vehicles using four motors, or wheeled road vehicles.  More are in development.


Have fun.
