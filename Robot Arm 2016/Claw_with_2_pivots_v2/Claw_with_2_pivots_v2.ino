
#define pin1  22  //these are the Arduino pins that we use to activate coils 1-4 of the stepper motor
#define pin2  13
#define pin3  12
#define pin4  11

#define pin5  23  //these are the Arduino pins that we use to activate coils 1-4 of the stepper motor
#define pin6  10
#define pin7  9
#define pin8  8

#define delaytime 8   //delay time in ms to control the stepper motor delaytime.
                      //Our tests showed that 8 is about the fastest that can yield reliable operation w/o missing steps
                   
int previous;
int previousclaw;


#include <Wire.h>
#include <Adafruit_MotorShield.h>


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);
int previous1;


void setup() {
  // initialize the 8 pin as an output:
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT); 
  pinMode(pin3, OUTPUT); 
  pinMode(pin4, OUTPUT);

  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT); 
  pinMode(pin7, OUTPUT); 
  pinMode(pin8, OUTPUT);
  
  Serial.begin(9600);
  delay(1000);


   AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(10);  // 10 rpm   
 

  
  previous1 = map(analogRead(A2),0,1023,0,200);
  previousclaw = map(analogRead(A0),0,1023,0,510);
  previous = map(analogRead(A1),0,1023,0,510);
  
}

void loop(){

  int valclaw = map(analogRead(A0),0,1023,0,510);
  if(valclaw > previousclaw+7) {
    forward1(valclaw - previousclaw);
    previousclaw = valclaw;
  }
  else if (valclaw < previousclaw-7) {
    backward1(previousclaw - valclaw);
    previousclaw = valclaw;
  }

  int val = map(analogRead(A1),0,1023,0,510);
  //int pivot1 = analogRead(A1);
  //Serial.println(pivot1);
  if(val > previous+7) {
    forward(val - previous);
    previous = val;
  }
  else if (val < previous-7) {
    backward(previous - val);
    previous = val;
  }


  int val1 = map(analogRead(A2),0,1023,0,200);
  // move a number of steps equal to the change in the
  // sensor reading
  int steps = val1 - previous1;
  //Serial.println(steps);
  if (steps > 0)
  {
     myMotor->step(steps, FORWARD, SINGLE);
  }
  else if (steps < 0)
  {
     myMotor->step(-steps, BACKWARD, SINGLE);
  }
  // remember the previous value of the sensor
  previous1 = val1;
  

  
}
