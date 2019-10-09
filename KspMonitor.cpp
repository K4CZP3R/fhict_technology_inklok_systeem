/*
   KspMonitor

   Simple watchdog and other arduino-related functions.
   ex: restarts arduino when critical error occures.

   By Kacper Serewis
   1.1
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
    }
    int freeRam() {

      return ESP.getFreeHeap();
    }
    void reset()
    {
      kspDebug->out("WILL RESTART ESP32!");
      delay(RESET_DELAY);
      kspDebug->out("Goodbye.");
      ESP.restart();
    }
  private:
    KspDebug* kspDebug;
};

#endif
