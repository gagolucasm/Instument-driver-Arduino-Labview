#include <SPI.h>
#include <Servo.h> 
char command;
String string;
#define led 13
#define lm A1
Servo servo1;
const int flexpin = 0; 
int flexposition;    
int servoposition;
const int chipSelectPin = 53;

void setup() 
{ 
  Serial.begin(9600);
  servo1.attach(2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();
  pinMode(chipSelectPin, OUTPUT);
  pinMode( 13, OUTPUT) ;
  pinMode( 12, OUTPUT) ;
} 


void loop() 
{ 
  long unsigned int adcData = readChannel(0);
  adcData = adcData & 0xFFFF;
  flexposition = (int)adcData;
  servoposition = map(flexposition, 950, 1800, 0, 180);
  servoposition = constrain(servoposition, 0, 180);
    if (Serial.available() > 0) 
    {string = "";}
    
    while(Serial.available() > 0)
    {
      command = ((byte)Serial.read());
      if(command == ':')
      {
        break;
      }
      
      else
      {
        string += command;
      }
      
      delay(1);
    }
    
    if(string == "SON")
    {
          Serial.println(flexposition);
          servo1.write(servoposition);
          serialFlush();
          digitalWrite( 12 , HIGH) ;
          digitalWrite( 13 , LOW) ;
    }
        else if(string == "SOFF")
    {
          Serial.println(flexposition);
          serialFlush();
          digitalWrite( 12 , HIGH) ;
          digitalWrite( 13 , LOW) ;
    }
    else{
        
          digitalWrite( 13 , HIGH) ;
          digitalWrite( 12 , LOW) ;
    }
    
 delay(200);

 }

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}   
  
unsigned int readChannel(byte thisCh) {
  byte inByte = 0;    
  long unsigned int result = 0;   
  digitalWrite(chipSelectPin, LOW);
  inByte = SPI.transfer(0x01);
  inByte = SPI.transfer(0xA0);
  result = inByte&0x0F;
  inByte = SPI.transfer(0x00);
  result = result<<8;
  result = result|inByte;
  digitalWrite(chipSelectPin, HIGH);
return(result);
}
