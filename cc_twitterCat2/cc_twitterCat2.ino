/*****************************
Creation Computation: Messaging
Twitter Cat Toy = you can play w/ bug the cat through twitter. 
Nadine Lessio and Bijun Chen
******************************/

#include <Servo.h>

byte incomingByte;            // from python
int command = 0;              // command (1 = open, 2 = close)
int startServo = 0;

boolean isRunning = false;
int thePos = 0;
int theInterval = 0;

boolean speedIncrease = false;

// Using a Class because I should learn some more things about classes. 

class Sweeper
{
  Servo servo;                // the servo
  int pos;                    // current servo position 
  int increment;              // increment to move for each interval
  int  updateInterval;        // interval between updates
  unsigned long lastUpdate;   // last update of position

public: 
  Sweeper(int interval)
  {
    updateInterval = interval;    // starting interval = 3
    increment = 1;
  }
  
  void Attach(int pin)
  {
    servo.attach(pin);          // what pin is the servo on?
  }
  
  void Detach()
  {
    servo.detach();             // remove servo?
  }
  
  void SweepIt(int highPos, int updateInterval)
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();                      // set last to current millis
      pos += increment;                           // incriment the position by X
      servo.write(pos);
      //Serial.println(pos);                        // print it
      //Serial.println(updateInterval);
      if ((pos >= highPos) || (pos <= 0))           // end of sweep
      {
        // reverse direction
        increment = -increment;
      }
    }
  }

  void Close(){
    servo.write(2);                           // set the servo to zero (turn it off)
  }

};

Sweeper sweeper1(5);        // initiate servo and pass the interval between each sweep update 

void setup() 
{
  Serial.begin(9600);       // begin serial communication
  Serial.setTimeout(20);    // set the timeout, the default is 1 second which is nuts.
  sweeper1.Attach(9);       // attach servo pin pas to Sweeper class
  sweeper1.Close();         // reset the servo
}

void loop() 
{
  readSerialData();
  if(isRunning){
    sweeper1.SweepIt(thePos,theInterval);
  } else{
    sweeper1.Close();
  }
  

}

void readSerialData(){
  if (Serial.available() > 0) {
    
    incomingByte = Serial.parseInt();   // use if testing from arduino input
    //incomingByte = Serial.read();         // use if live
    command = incomingByte;
    startServo = command;
    Serial.println("!readSerialData");
    Serial.println(startServo);
    servoLogic();
  }
}

void servoLogic(){
  // In here so that it only exectues once rather than looping. 


  /****************************** 
  servo commands:
  2 = start (default to small sweep)
  3 = increase speed
  4 = decrease speed
  5 = increase positioning
  6 = decrease positiing
  7 = close. 
  ****************************/

  if(startServo == 2){
    Serial.println("starting Servo");
    // set initial angle and speed, which is small and slow.
    theInterval = 10;
    thePos = 50;
    isRunning = true;
  } 

  // set the upper and lower bounds for speed. Pos is in the class. 

  if(theInterval <= 3){
    theInterval = 3;
  }else if(theInterval >= 10){
    theInterval = 10;
  }
  
  // set the servo logic if it is running
  if(isRunning){
    if(startServo == 4){
      // decrease the speed by increasing the interval of the sweep
      theInterval++;
      Serial.println("speed--:");
      Serial.println(theInterval);
    } else if(startServo == 3){
      // increase the speed by decreasing the interval of the sweep.
      theInterval--;
      Serial.println("speed++");
      Serial.println(theInterval);
    }else if(startServo == 5){
      // increase the servo position by 10.
      thePos+=10;
      Serial.println("pos++");
      //Serial.println(thePos);
    } else if(startServo == 6){
      // decrease the servo position by 20. This is for "wind down" afer X number of tweets. 
      thePos-=10;
      Serial.println("pos--");
      Serial.println(thePos);
    } else if(startServo == 7){
      // shut it off. 
      isRunning = false;
    }
  }
  // print the interval and pos
  //Serial.println(theInterval);
  //Serial.println(thePos);
  
  
  
}