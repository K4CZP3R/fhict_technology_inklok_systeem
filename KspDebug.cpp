/*
 * KspDebug
 * 
 * Module to print debug messages.
 * Uses KspSerial to print to software serial.
 * 
 * By Kacper Serewis
 * 1.1
 */

#ifndef KspDebug_h
#define KspDebug_h

#include "Arduino.h"
#include "KspSerial.cpp"

class KspDebug
{
  public:
    KspDebug(bool useSoftwareSerial, const char* moduleName)
    {
      this->kspSerial = useSoftwareSerial ? new KspSerial(true) : new KspSerial(false);
      this->moduleName = moduleName;
    }
    void init() {
      kspSerial->kspOpenSerial();

    }
    void set_decor_to(int decor_id)
    {
      char* selected_decor = "";
      switch (decor_id)
      {
        case -1:
          selected_decor = "\u001b[0m"; //reset
          break;
        case 0:
          selected_decor = "\u001b[1m"; //bold
          break;
        case 1:
          selected_decor = "\u001b[4m"; //underline
          break;
        case 2:
          selected_decor = "\u001b[7m"; //reversed
          break;
      }
      kspSerial->kspPrint(selected_decor);
    }
    void set_color_to(int color_id)
    {
      char* selected_color = "";
      switch (color_id) {
        case -1:
          selected_color = "\u001b[0m"; //reset
          break;
        case 0:
          selected_color = "\u001b[31m"; //red
          break;
        case 1:
          selected_color = "\u001b[32m"; //green
          break;
        case 2:
          selected_color = "\u001b[34m"; //blue
          break;
        case 3:
          selected_color = "\u001b[33m"; //yellow
          break;
        case 4:
          selected_color = "\u001b[36m";  //cyan
          break;
      }

      kspSerial->kspPrint(selected_color);
    }
    void plain_out(const char* content, bool new_line=true)
    {
      kspSerial->kspPrint(content, new_line);
    }
    void plain_out(String content, bool new_line=true)
    {
      kspSerial->kspPrint(content, new_line);
    }
    void out(const char* content)
    {
      printModuleName();
      kspSerial->kspPrint(content, true);
    }
    void out(char content) {
      printModuleName();
      kspSerial->kspPrint(content, true);
    }
    void out(String content) {
      printModuleName();
      kspSerial->kspPrint(content, true);
    }
    void out(int content) {
      printModuleName();
      kspSerial->kspPrint(content, true);
    }
    void outHex(int content)
    {
      printModuleName();
      kspSerial->kspPrintHex(content, true);
    }
    void outDescHex(const char* desc, int content)
    {
      printModuleName();
      kspSerial->kspPrint(desc);
      kspSerial->kspPrint(": ");
      kspSerial->kspPrintHex(content, true);
    }
  private:
    KspSerial* kspSerial;
    const char* moduleName;

    void printModuleName() {
      set_color_to(3);
      set_decor_to(0);
      kspSerial->kspPrint("[");
      kspSerial->kspPrint(moduleName);
      kspSerial->kspPrint("] ");
      kspSerial->kspPrint("<");
      kspSerial->kspPrint(String(ESP.getFreeHeap()/1000));
      kspSerial->kspPrint("kb free> ");
      set_color_to(-1);
    }
};
#endif
