/*
 * KspMonitor
 * 
 * Simple watchdog and other arduino-related functions.
 * ex: restarts arduino when critical error occures.
 * 
 * By Kacper Serewis
 * 1.0
 */

#ifndef KspMonitor_h
#define KspMonitor_h

#include "Arduino.h"
#include "KspDebug.cpp"

#define RESET_PIN A5
#define RESET_DELAY 2000
class KspMonitor
{
  public:
    KspMonitor()
    {
      this->kspDebug = new KspDebug(true, "KspMonitor");
    }
    void init()
    {
      kspDebug->init();
      kspDebug->out("Debug inited!");

      kspDebug->out("Setting reset pin...");
      digitalWrite(RESET_PIN, HIGH);
      pinMode(RESET_PIN, OUTPUT);
      kspDebug->out("OK!");
    }

    void reset()
    {
      kspDebug->out("WILL RESTART ARDUINO");
      delay(RESET_DELAY);
      kspDebug->out("Goodbye.");
      digitalWrite(RESET_PIN, LOW);
      kspDebug->out("You can't see this message");
    }
  private:
    KspDebug* kspDebug;
};

#endif
