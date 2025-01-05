#include <Servo.h>
#define MOTOR_ARDUINO Serial1


int lightsensorPin = A0;

int canSensorPin = 2;
int IRdigitalPin = 6;
int razerPin = 9;
unsigned long previous, current;


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


bool isObjectDetected(int ifredSensor) {
  return !digitalRead(ifredSensor);
}

void setMotor(int index, int angle) {
/**
 * 0: 센서칸 뚜껑
 * 1: 일반쓰레기 뚜껑
 * 2: 캔 쓰레기 뚜껑
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
