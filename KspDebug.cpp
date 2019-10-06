/*
 * KspDebug
 * 
 * Module to print debug messages.
 * Uses KspSerial to print to software serial.
 * 
 * By Kacper Serewis
 * 1.0
 */

#ifndef KspDebug_h
#define KspDebug_h

#include "Arduino.h"
#include "KspSerial.cpp"

#define SOFTWARE_RX 2
#define SOFTWARE_TX 3
#define SOFTWARE_BDRATE 9600



class KspDebug
{
  public:
    KspDebug(bool useSoftwareSerial, const char* moduleName)
    {
      this->kspSerial = useSoftwareSerial ? new KspSerial(SOFTWARE_BDRATE, SOFTWARE_RX, SOFTWARE_TX) : new KspSerial(SOFTWARE_BDRATE);
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
      set_color_to(-1);
    }
};
#endif
