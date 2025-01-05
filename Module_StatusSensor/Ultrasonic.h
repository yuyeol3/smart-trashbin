

#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include "Arduino.h"

class Ultrasonic {
public:
  Ultrasonic(int _trigPin, int _echoPin)
  : trigPin(_trigPin), echoPin(_echoPin)
  { 
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  float getDistance() {
    digitalWrite(trigPin, 1);
    delay(10);
    digitalWrite(trigPin, 0);

    float duration = pulseIn(echoPin, 1, 100000L);
    float distance = duration * 340 / 10000 / 2;
    return distance;
  }
private:
  int trigPin, echoPin;
};


const Ultrasonic us1(43, 42);
// const Ultrasonic us2(41, 40);

#endif