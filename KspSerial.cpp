/*
 * KspSerial
 * 
 * Simple serial wrapper, created to unify software serial and the "hardware" serial.
 * (Makes it easier to use software serial like "hardware" serial)
 * 
 * By Kacper Serewis
 * 1.0
 */

#ifndef KspSerial_h
#define KspSerial_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class KspSerial
{
  public:
    KspSerial(int bdRate) 
    {
      this->bdRate = bdRate;
    };

    KspSerial(int bdRate, int rx, int tx) 
    {
      this->rx = rx;
      this->tx = tx;
      this->bdRate = bdRate;
      this->useSoftSerial = true;
    }
    int kspSerialRead()
    {
      return useSoftSerial ? softSerial->read() : Serial.read();
    }
    int kspSerialAvailable()
    {
      return useSoftSerial ? softSerial->available() : Serial.available();
    }
    void kspOpenSerial() 
    {
      if (useSoftSerial) 
      {
        this->softSerial = new SoftwareSerial(rx, tx);
      }
      
      this->useSoftSerial ? softSerial->begin(bdRate) : Serial.begin(bdRate);
    };

    void kspPrint(const char* content, bool newLine = false) 
    {
      useSoftSerial ? softSerial->print(content) : Serial.print(content);
      if (newLine) useSoftSerial ? softSerial->println() : Serial.println();
    }
    void kspPrint(String content, bool newLine = false) 
    {
      useSoftSerial ? softSerial->print(content) : Serial.print(content);
      if (newLine) useSoftSerial ? softSerial->println() : Serial.println();
    }
    void kspPrint(int content, bool newLine = false) 
    {
      useSoftSerial ? softSerial->print(content) : Serial.print(content);
      if (newLine) useSoftSerial ? softSerial->println() : Serial.println();
    }
    void kspPrint(char content, bool newLine = false) 
    {
      useSoftSerial ? softSerial->print(content) : Serial.print(content);
      if (newLine) useSoftSerial ? softSerial->println() : Serial.println();
    }
    void kspPrintHex(int content, bool newLine = false) 
    {
      useSoftSerial ? softSerial->print(content, HEX) : Serial.print(content, HEX);
      if (newLine) useSoftSerial ? softSerial->println() : Serial.println();
    }
  private:
    int rx;
    int tx;
    int bdRate;
    bool useSoftSerial = false;
    SoftwareSerial* softSerial;
};

#endif
