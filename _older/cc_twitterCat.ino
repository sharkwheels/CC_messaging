/*****************************
Creation Computation: Messaging
Twitter Cat Toy = you can play w/ bug the cat through twitter. 
******************************/

#include <Servo.h>

byte incomingByte;            // from python
int command = 0;              // command (1 = open, 2 = close)
int startServo = 0;

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
    updateInterval = interval;
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
  
  void SweepIt()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();                      // set last to current millis
      pos += increment;                           // incriment the position by X
      servo.write(pos);
      Serial.println(pos);                        // print it
      if ((pos >= 180) || (pos <= 0))           // end of sweep
      {
        // reverse direction
        increment = -increment;
      }
    }
  }

  void Close(){
    servo.write(0);                           // set the servo to zero (turn it off)
  }

};

Sweeper sweeper1(5);        // initiate servo and pass the interval between each sweep update 

void setup() 
{
  Serial.begin(9600);       // begin serial communication
  Serial.setTimeout(20);    // set the timeout, the default is 1 second which is nuts.

  sweeper1.Attach(9);       // attach servo pin pas to Sweeper class


}

void loop() 
{
  if (Serial.available() > 0) {
    
    incomingByte = Serial.parseInt();   // use if testing from arduino input
    //incomingByte = Serial.read();         // use if live
    command = incomingByte;
    startServo = ServoGo(command);

  }

  if(startServo == 2){      // if the command is two
    sweeper1.SweepIt();      // call the sweep motion
  } else {
    sweeper1.Close();       // stop the servo
  }
}

int ServoGo(int com){
  Serial.println("!inServoGo");   // track that you are in this function
  Serial.println(com);            // print the command
  return com;                     // return the command. 
}
