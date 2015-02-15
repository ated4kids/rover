/**
Bruno Santos, 2013
feiticeir0@whatgeek.com.pt
modified by Gaspar Torriero 2015

DC motors - 2x with a L298 Dual H-Bridge Motor Driver
2 x HC-SR04 ultrasound sensors
**/

#include <Wire.h>
//Ultrasound sensor
#define ECHOPINR 12
#define TRIGPINR 11
#define ECHOPINL 7
#define TRIGPINL 6
//Minimum distance from obstacle
int minDistance = 10;
int medDistance = 20;
int cmR;
int cmL;

//TDC Motors
//Define Pins
//Motor A
int enableA = 10;
int pinA1 = 2;
int pinA2 = 3;

//Motor B
int enableB = 9;
int pinB1 = 4;
int pinB2 = 5;
//define time for run
// in milliseconds
int running = 10000; //10 secons
boolean play;

void setup() {
  //Serial.begin (9600);
  //Configue sensor pin modes
  pinMode(ECHOPINR, INPUT);
  pinMode(TRIGPINR, OUTPUT);
  pinMode(ECHOPINL, INPUT);
  pinMode(TRIGPINL, OUTPUT);

  //configure motor pin modes
  pinMode (enableA, OUTPUT);
  pinMode (pinA1, OUTPUT);
  pinMode (pinA2, OUTPUT);

  pinMode (enableB, OUTPUT);
  pinMode (pinB1, OUTPUT);
  pinMode (pinB2, OUTPUT);

  play = true;

}
//Defining functions so it's more easy

//motor functions
void motorAforward() {
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, LOW);
}
void motorBforward() {
  digitalWrite (pinB1, LOW);
  digitalWrite (pinB2, HIGH);
}
void motorAbackward() {
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, HIGH);
}
void motorBbackward() {
  digitalWrite (pinB1, HIGH);
  digitalWrite (pinB2, LOW);
}
void motorAstop() {
  digitalWrite (pinA1, HIGH);
  digitalWrite (pinA2, HIGH);
}
void motorBstop() {
  digitalWrite (pinB1, HIGH);
  digitalWrite (pinB2, HIGH);
}
void motorAcoast() {
  digitalWrite (pinA1, LOW);
  digitalWrite (pinA2, LOW);
}
void motorBcoast() {
  digitalWrite (pinB1, LOW);
  digitalWrite (pinB2, LOW);
}
void motorAon() {
  digitalWrite (enableA, HIGH);
}
void motorBon() {
  digitalWrite (enableB, HIGH);
}
void motorAoff() {
  digitalWrite (enableA, LOW);
}
void motorBoff() {
  digitalWrite (enableB, LOW);
}
// Movement functions
void forward (int duration) {
  motorAforward();
  motorBforward();
  delay (duration);
}
void backward (int duration) {
  motorAbackward();
  motorBbackward();
  delay (duration);
}
void hardleft (int duration) {
  motorBbackward();
  motorAforward();
  delay (duration);
}
void hardright (int duration) {
  motorBforward();
  motorAbackward();
  delay (duration);
}
void left (int duration) {
  motorBstop();
  motorAforward();
  delay (duration);
}
void right (int duration) {
  motorBforward();
  motorAstop();
  delay (duration);
}
void coast (int duration) {
  motorAcoast();
  motorBcoast();
  delay (duration);
}
void breakRobot (int duration) {
  motorAstop();
  motorBstop();
  delay (duration);
}
void disableMotors() {
  motorAoff();
  motorBoff();
}
void enableMotors() {
  motorAon();
  motorBon();
}

void loop() {

  while (play) {
    enableMotors();
    //   hardright (1);

    //Measure obstacle distance in cm

    digitalWrite(TRIGPINR, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPINR, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPINR, LOW);
    float distanceR = pulseIn(ECHOPINR, HIGH);
    cmR = distanceR / 58;

    digitalWrite(TRIGPINL, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPINL, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPINL, LOW);
    float distanceL = pulseIn(ECHOPINL, HIGH);
    cmL = distanceL / 58;

// decide where to go

    if ((cmR < minDistance) || (cmL < minDistance)) {
      if (cmR < cmL) {
        hardleft(5);
      }
      else if (cmL < cmR) {
        hardright (5);
      }
    }
    else if ((cmR < medDistance) || (cmL < medDistance)) {
      if (cmR < cmL) {
        left(5);
      }
      else if (cmL < cmR) {
        right (5);
      }
    }
    else {
      forward (5);
    }
  }
}
