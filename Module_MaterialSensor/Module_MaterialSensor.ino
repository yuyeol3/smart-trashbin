#include <Servo.h>
#define MOTOR_ARDUINO Serial1
// int serviPin[] = { 10, 11, 12, 13 };
int lightsensorPin = A0;
// int speakerPin = 9;
int canSensorPin = 2;
int IRdigitalPin = 6;
// int TOFsensorPin[] = { 2, 3 };
// int ultraTrig = 22;
// int ultraEcho = 23;
int razerPin = 9;
// int IRanalogPin = 54;
// int TOFPin[] = {18, 19}; // 18 is yellow, 19 is white
unsigned long previous, current;




//디지털 핀 설정

void setup() {
  Serial.begin(9600);
  // Serial1.begin(9600);
  MOTOR_ARDUINO.begin(9600);

  pinMode(razerPin, OUTPUT);
  pinMode(canSensorPin, INPUT);
  pinMode(IRdigitalPin, INPUT);
  // pinMode(IRanalogPin, INPUT);
  digitalWrite(razerPin, HIGH);   // Be careful

  previous = millis();
}

// float ultraSonicDistance() {   // need more power
//   digitalWrite(ultraTrig, HIGH);
//   delay(10);
//   digitalWrite(ultraTrig, LOW);

//   float duration = pulseIn(ultraEcho, HIGH);

//   float distance = duration * 0.17;   // unit is meter

//   return distance;
// }

// int distanceTOF() {
//   if (Serial.available()) {
//     return Serial1.read();
//   }
// }

int isPlastic() {
  int light = analogRead(lightsensorPin);

  Serial.println(light);
  delay(100);
  if (light < 550)
    return 1;
  return 0;
}

bool isMetal() {
  return !digitalRead(canSensorPin);
}

// bool isObjectDetected(int ifredSensor) {
//   tone(speakerPin, 100, 1);
//   return !digitalRead(ifredSensor);
// }


bool isObjectDetected(int ifredSensor) {
  return !digitalRead(ifredSensor);
}

void setMotor(int index, int angle) {
  /*
  0 -> sensing part to slope
  1 -> others
  2 -> pet

  */
  Serial.print("REQUEST : ");
  Serial.print(index);
  Serial.print(" ");
  Serial.print(angle);
  Serial.println();
  MOTOR_ARDUINO.println(String(index) + "," + angle);
  delay(1000);

}

void loop() {
  if (isObjectDetected(IRdigitalPin)) {
    delay(1000);
    if (isMetal()) {
      Serial.println("metal");
      setMotor(2, 110);
      delay(100);
      setMotor(0, 170);
    } else if (isPlastic()) {
      Serial.println("plastic");
      setMotor(0, 170);
    } else {
      Serial.println("others");
      setMotor(1, 90);
      delay(100);
      setMotor(0, 170);
    }
  
    delay(1000);

    setMotor(0, 0);
    delay(50);
    setMotor(1, 180);
    delay(50);
    setMotor(2, 180);
  }
}
