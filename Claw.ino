#include <Servo.h>

#define GND_PIN 10
#define ECHO_PIN 11
#define TRIG_PIN 12
#define VCC_PIN 13

#define SERVO_PIN 9
#define CLOSE_ANGLE 45
#define OPEN_ANGLE 120

#define DISTANCE_SCALE 29.1
#define CLOSE_RANGE 12

Servo servo;
int keepClosed = 0;

void setup() {
  Serial.begin (9600);          // set data transmission rate to communicate with computer
  pinMode(VCC_PIN, OUTPUT) ;    // tell pin 13 it is going to be an output
  digitalWrite(VCC_PIN, HIGH) ; // tell pin 13 to output HIGH (+5V)
  pinMode(ECHO_PIN, INPUT);     // tell pin 11 it is going to be an input
  pinMode(TRIG_PIN, OUTPUT);    // tell pin 12 it is going to be an output
  pinMode(GND_PIN, OUTPUT) ;    // tell pin 10 it is going to be an output
  digitalWrite(GND_PIN, LOW) ;  // tell pin 10 to output LOW (0V, or ground)
  
  servo.attach(SERVO_PIN);
  delay(500);
  servo.write(OPEN_ANGLE);         
}

void closeClaw(Servo useServo, int closingSpeed, int closeAngle, int initialAngle) {
  for (int pos = initialAngle; pos >= closeAngle; pos -= closingSpeed) {
   useServo.write(pos);           
   delay(15); 
  }
}

// contains program for variety round
void varietyRound(Servo useServo, int sonarDistance) {
  // check to close/open claw
  if (sonarDistance < CLOSE_RANGE && keepClosed != 1) {
    useServo.write(CLOSE_ANGLE);
    delay(500);
    keepClosed = 1;
    delay(500);
  } else if (keepClosed == 1 && sonarDistance < CLOSE_RANGE) {
    keepClosed = 0;
    useServo.write(OPEN_ANGLE);  
    delay(500);  
  } else if (keepClosed == 0) {
    useServo.write(OPEN_ANGLE);  
  } else {
    useServo.write(CLOSE_ANGLE);
  }
}

// contains program for the challenge round
void challengeRound(Servo useServo, int sonarDistance) {
  // check to close/open claw
  if (sonarDistance < CLOSE_RANGE && keepClosed != 1) {
    closeClaw(useServo, 1, CLOSE_ANGLE, OPEN_ANGLE);
    delay(500);
    keepClosed = 1;
    delay(500);
  } else if (keepClosed == 1 && sonarDistance < CLOSE_RANGE) {
    keepClosed = 0;
    useServo.write(OPEN_ANGLE);  
    delay(500);  
  } else if (keepClosed == 0) {
    useServo.write(OPEN_ANGLE);  
  } else {
    useServo.write(CLOSE_ANGLE);
  }
}

void loop() {
  int duration, distance;                     //defining variables
  digitalWrite(TRIG_PIN, HIGH);               //set trigger pin to HIGH
  delayMicroseconds(1000);                    //wait 1000 microseconds
  digitalWrite(TRIG_PIN, LOW);                //set trigger pin to LOW
  duration = pulseIn(ECHO_PIN, HIGH);         //read echo pin
  distance = (duration / 2) / DISTANCE_SCALE; //compute distance from duration of echo Pin


  // *** CHANGE FUNCTION FOR DIFFERENT ROUND HERE *** \\
   
  varietyRound(servo, distance);
  //challengeRound(servo, distance);

  // show distance on screen
  if (distance >= 400 || distance <= 0) {
    Serial.println("Out of range");
  } else {
    Serial.print(distance);  
    Serial.println(" cm");
  }

  // set loop refresh
  delay(500);
}

