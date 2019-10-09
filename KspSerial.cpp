/*
 * KspSerial
 * 
 * Simple serial wrapper, created to unify software serial and the "hardware" serial.
 * (Makes it easier to use software serial like "hardware" serial)
 * 
 * By Kacper Serewis
 * 1.1
 */

#ifndef KspSerial_h
#define KspSerial_h

#include "Arduino.h"
#include <SoftwareSerial.h>

#define SOFT_RX 12
#define SOFT_TX 14
#define SOFT_BDRATE 57600
#define HARD_BDRATE 115200

class KspSerial
{
  public:
    KspSerial(bool useSoftSerial)
    {
      this->useSoftSerial = useSoftSerial;
    }
    int kspSerialRead()
    {
      return useSoftSerial ? softSerial.read() : Serial.read();
    }
    int kspSerialAvailable()
    {
      return useSoftSerial ? softSerial.available() : Serial.available();
    }
    void kspOpenSerial() 
    {
      this->useSoftSerial ? softSerial.begin(SOFT_BDRATE, SOFT_TX, SOFT_RX, SWSERIAL_8N1, false, 95, 11) : Serial.begin(HARD_BDRATE);
    };

    void kspPrint(const char* content, bool newLine = false) 
    {
      useSoftSerial ? softSerial.print(content) : Serial.print(content);
      if (newLine) useSoftSerial ? softSerial.println() : Serial.println();
    }
    void kspPrint(String content, bool newLine = false) 
    {
      useSoftSerial ? softSerial.print(content) : Serial.print(content);
      if (newLine) useSoftSerial ? softSerial.println() : Serial.println();
    }
    void kspPrint(int content, bool newLine = false) 
    {
      useSoftSerial ? softSerial.print(content) : Serial.print(content);
      if (newLine) useSoftSerial ? softSerial.println() : Serial.println();
    }
    void kspPrint(char content, bool newLine = false) 
    {
      useSoftSerial ? softSerial.print(content) : Serial.print(content);
      if (newLine) useSoftSerial ? softSerial.println() : Serial.println();
    }
    void kspPrintHex(int content, bool newLine = false) 
    {
      useSoftSerial ? softSerial.print(content, HEX) : Serial.print(content, HEX);
      if (newLine) useSoftSerial ? softSerial.println() : Serial.println();
    }
  private:
    int rx;
    int tx;
    int bdRate;
    bool useSoftSerial = false;
    SoftwareSerial softSerial;
};

#endif
