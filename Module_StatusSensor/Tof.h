#include "Arduino.h"

#ifndef TOF_H
#define TOF_H

template<typename T>
class Tof {
public:
  Tof() {}

  Tof(T *_serial, String *setupProperties, int length) {
    serial = _serial;
    serial->begin(9600);
      for (int i = 0; i < length; ++i) {
        serial->write(setupProperties[i].c_str());
        // changeSetup(setupProperties[i]);
      }
  }

  int getDistance() {
    changeSetup(String("r6#"));
    if (serial->available()) {
      return serial->parseInt();
    }

    return 0;
  }

  int changeSetup(String setup) {
    serial->write(setup.c_str());
      // return serial->read();
  }
  

private:
  T *serial;
};

// ToF250 통신을 위한 시리얼
// SoftwareSerial ToF250(TX_PIN, RX_PIN);

String setupString[] = {
  // "s6-0#",
  // "s7-0#",
  // "rst#",
};

Tof<HardwareSerial> tof(&Serial2, setupString, sizeof(setupString)/sizeof(*setupString));

#endif