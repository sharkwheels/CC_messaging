/*****************************
Creation Computation: Messaging
Twitter Cat Toy = you can play w/ bug the cat through twitter. 
Nadine Lessio and Bijun Chen
******************************/

 /****************************** 
  servo commands:
  2 = start (default to small sweep)
  3 = increase speed
  4 = decrease speed
  5 = increase positioning
  6 = decrease positiing
  7 = close. 
  ****************************/


#include <Servo.h>

byte incomingByte;            // from python
int command = 0;              // command (1 = open, 2 = close)
int startServo = 0;
boolean isRunning = false;

Servo servo; 
int pos;              // current servo position 
int increment;        // increment to move for each interval
int  updateInterval;      // interval between updates
unsigned long lastUpdate; // last update of position
int posLimit;

int windDown = 5;


void setup() 
{
  Serial.begin(9600);       // begin serial communication
  Serial.setTimeout(20);    // set the timeout, the default is 1 second which is nuts.
  updateInterval = 10;
  posLimit = 60;
  increment = 2;

  servo.attach(9);
  servo.write(90);
}

void loop() 
{
  readSerialData();
  

  if(isRunning){
    
    Serial.println(updateInterval);
    Serial.println(pos);
    

    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
      pos += increment;
      servo.write(pos);
      Serial.println(updateInterval);
      if ((pos >= posLimit) || (pos <= 0)) // end of sweep
      {
        // reverse direction
        increment = -increment;
      }
    }
  }else{
    servo.write(90);
    //servo.detach();  // this always makes it get stuck. Efffffff it. 

  }

}

void readSerialData(){
  if (Serial.available() > 0) {
    
    //incomingByte = Serial.parseInt();   // use if testing from arduino input
    incomingByte = Serial.read();         // use if live
    command = incomingByte;
    startServo = command;
    Serial.println("!readSerialData");
    Serial.println(startServo);
    servoLogic();
  }
}


void servoLogic(){
  if(!isRunning && startServo == 2){
    //servo.attach(9);
    isRunning = true;
  } 

  if(isRunning){
    if(startServo == 7){
      isRunning = false;
    } else if(startServo == 3){
      
      // increase the speed by decreasing the interval of the sweep.
      if(updateInterval <= 3){
        updateInterval = 3;
      }else{
        updateInterval--;
      }
      
    } else if(startServo == 4){
      // decrease the speed by increasing the interval of the sweep
      if(updateInterval >= 20){
        updateInterval = 20;
      }else{
        updateInterval++;
      }
      
    } else if(startServo == 5){
      if(posLimit >= 180){
        posLimit = 180;
      }else{
        posLimit+=20;
      }
      
    } else if(startServo == 6){
      // decrease the positiinging
      if(posLimit <= 0){
        posLimit = 0;
      }else{
        posLimit-=20;
      }
      
    } else if(startServo == 8){
      /// some kinda wind down
      posLimit = 60;
      updateInterval = 10;
    }else{
        Serial.println("nothing");
    }
  }
}

