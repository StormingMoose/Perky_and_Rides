
/*

UPDATED NOV 2019  MOVED ARRAY VARIABLES TO BOTTOM OF PRIVATE DECLARATION




       LEGAL NOTICE:
      Meccano™, MeccaBrain™, MeccanoTech™ and Meccanoid™ are trademarks of Meccano.  All rights reserved.     
      The information contained in this document is Proprietary to Spin Master Ltd.   
      This information should not be used in the creation and sale of any commercial product.”
*/
#ifndef MeccaBrain_h
#define MeccaBrain_h

#include "Arduino.h"

class MeccaBrain{


    public:
        MeccaBrain(int pin);
        
        byte outputByteInfo(int num);
        byte inputByteInfo();
        byte checkSumByteInfo();
        
        char moduleTypeInfo(int num);
        byte moduleNumInfo();
        byte getLEDorder();
        byte getLEDbyte1();
        byte getLEDbyte2();    

        void setLEDColor(byte red, byte green, byte blue, byte fadetime);
        
        void communicate();        
        void sendByte(byte servoData);
        byte receiveByte();
        byte calculateCheckSum(byte Data1, byte Data2, byte Data3, byte Data4);
        
        byte inputBytesInfo(int servoNum);
        void setServoColor(int servoNum, byte color);
        ;
        void setServotoLIM(int servoNum);
        byte getServoPosition(int servoNum);
        
        byte servoPositionsInfo(int servoNum);
        void setServoPosition(int servoNum, byte pos);

    private:
        int _pmwPin;
        int servoNumber;
        int inputCounter = 0;
        int moduleNum = 0;
        int printModNum = 0;
        int ledOrder = 0;
        int bitDelay = 417;
        int receiveArray[8];
        
        
        byte inputByte;
        
        byte checkSum;
        byte servoPos;
        byte LEDoutputByte1 = 0x00;
        byte LEDoutputByte2 = 0x47;
        byte header = 0xFF;
        byte outputByte[4];
        byte printOutputByte[4];
        
        char moduleType[4];
		byte inputBytes[20];     
        int servo1Positions[20];
        int servo2Positions[20];
        int servo3Positions[20];
        int servo4Positions[20];

    
        
};

#endif
