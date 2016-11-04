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









/*

/// GLOBALS ///////////////////////////////////////

#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN 12

// replace neopixels w/ fast LED
// think the servo class is probably ok. 
// except we're not using servos. christ. 


Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_GRB + NEO_KHZ800);


class Sweeper {
  Servo servo;              // the servo
  public:
    Sweeper(){
      // eventually stuff
    }

  void Attach(int pin){
    servo.attach(pin);
  }
  void Detach(){
    servo.detach();
  }
  
  void Open(){
   servo.write(180);
  }

  void Close(){
    servo.write(0);
  }
};

/// serial from python command ///
byte incomingByte; // from python
int command = 0; // command (1 = open, 2 = close)

/// servo ////
static const uint8_t servoPins[] = {5,6};

int startServo = 0;
Sweeper sweeper1;

/// FUNCTIONS ///////////////////////////////////////

void setup(void){
  Serial.begin(9600); // begin serial communication
  Serial.setTimeout(20); // set the timeout, the default is 1 second which is nuts.
  sweeper1.Attach(servoPins[0]); // attach servo pin 5
  
  strip.begin();
  strip.setBrightness(100); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {

  if (Serial.available() > 0) {
    //incomingByte = Serial.parseInt(); // use if testing from arduino input
    incomingByte = Serial.read(); // use if live
    command = incomingByte;
    startServo = ServoGo(command);
    /// have 4 flowers. 
      /// need a pin
      /// command 

  } 
  
  if (startServo == 1){
    sweeper1.Open();
    colorWipe(strip.Color(0, 255, 0), 0); // Green
  } else if (startServo == 2){
    sweeper1.Close();
    colorWipe(strip.Color(255, 0, 0), 0); // Red
  }

  
}

int ServoGo(int com){
  Serial.println("!inServoGo");
  Serial.println(com);
  return com;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


/*void Update(){
    if((millis() - lastUpdate) > updateInterval){
      lastUpdate = millis();
      pos += increment;
      servo.write(pos);
      //Serial.println(pos);
      if ((pos >= 180) || (pos <= 0)){
        increment = -increment;
      }
    }
  }*/
  
  /*
  /* interval = 15;
      updateInterval = interval;
      increment = 10;*/
 /*
 /* int pos;              // current servo position 
  int increment;        // increment to move for each interval
  int updateInterval;      // interval between updates
  int interval;
  unsigned long lastUpdate; // last update of position
  */


/*
byte byteRead;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    byteRead = Serial.read();
    Serial.println(byteRead);
  }
  delay(20);

}
*/
