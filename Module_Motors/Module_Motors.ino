#include <Servo.h>
#include "Ultrasonic.h"
#define SENSOR_ARDUINO Serial1

Servo servos[4];

long prev, current;
bool open =  false;

void setup() {
  // put your setup code here, to run once:
  SENSOR_ARDUINO.begin(9600);
  Serial.begin(9600);
  servos[0].attach(11);
  servos[1].attach(10);
  servos[2].attach(8);
  servos[3].attach(9);
  prev = millis();

  servos[0].write(90);
  servos[1].write(0);
  servos[2].write(180);
  servos[3].write(180);

  SENSOR_ARDUINO.setTimeout(1000);
}

void motorWriteSmooth(Servo &s, int angle) {
  int current_angle = s.read();
  int dA = angle - current_angle;
  int abs_dA = abs(dA);

  if (dA < 0) {
    for (int i = current_angle; i >= angle; i--) {
      s.write(i);
      delay(5);
    }

  } else {
    for (int i = current_angle; i < angle; i++) {
      s.write(i);
      delay(5);
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  // if (SENSOR_ARDUINO.available()) {


  //   // delay(100);
  //   int index = SENSOR_ARDUINO.parseInt() + 1;
  //   delay(500);
  //   int angle = SENSOR_ARDUINO.parseInt();
  //   Serial.println(String(index) + " " + angle);
  //   servos[index].write(angle);
  // }
  if (SENSOR_ARDUINO.available()) {
      String data = SENSOR_ARDUINO.readStringUntil('\n'); // 한 줄의 데이터를 읽음
      int separatorIndex = data.indexOf(',');            // 쉼표로 구분된 데이터 확인
      if (separatorIndex != -1) {
          int index = data.substring(0, separatorIndex).toInt() + 1;
          int angle = data.substring(separatorIndex + 1).toInt();

          if (index >= 0 && index < 4 && angle >= 0 && angle <= 180) {
              motorWriteSmooth(servos[index], angle);
              // servos[index].write(angle);
              Serial.println("Index: " + String(index) + ", Angle: " + String(angle));
          } else {
              Serial.println("Invalid data: " + data);
          }
      } else {
          Serial.println("Malformed data: " + data);
      }
      delay(500);
  }

  current = millis(); 

  if (us1.getDistance() <= 4 && !open) {
    // servos[0].write(150);
    motorWriteSmooth(servos[0], 150);
    open = true;
    prev = current;
  }

  if (open && current - prev >= 3500) {
    prev = current;
    open = false;
    // servos[0].write(90);
    motorWriteSmooth(servos[0], 90);
  }
  // servos[1].write(-10);
  
}
