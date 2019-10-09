/*
 * KspCommunication
 * 
 * Communication protocol made special for this app.
 * uses Serial port to communicate with computer.
 * 
 * By Kacper Serewiś 
 * 1.1
 */

#ifndef KspCommunication_h
#define KspCommunication_h

#include "Arduino.h"
#include "KspDebug.cpp"
#include "KspSerial.cpp"

#define START_MAGIC 0x3c
#define END_MAGIC 0x3e

class KspCommunication
{
  public:
    KspCommunication()
    {
      this->kspDebug = new KspDebug(true, "KspCommunication");
      this->kspSerial = new KspSerial(false);
      
    };
    void init()
    {
      kspDebug->init();  
      kspDebug->out("Debug inited!");
      kspSerial->kspOpenSerial();
      kspDebug->out("Communication serial opened!");
    };

    void kspSendCommand(String command)
    {
      kspDebug->out("Will send following command:");
      kspDebug->out(">" + command);
      kspSerial->kspPrint((char)START_MAGIC);
      kspSerial->kspPrint(command);
      kspSerial->kspPrint((char)END_MAGIC);
    }

    String kspReceiveCommand()
    {
      char recv_buf[128];
      int buf_n = 0;
      bool end_magic_found = false;
      bool start_magic_found = false;
      while(kspSerial->kspSerialAvailable() > 0 && end_magic_found == false)
      {
        int cur_byte = kspSerial->kspSerialRead();
        kspDebug->outDescHex("recv",cur_byte);
        if(!start_magic_found)
        {
          if(cur_byte == START_MAGIC)
          {
            kspDebug->out("START MAGIC found!");
            start_magic_found = true;
          }
        }

        else
        {
          recv_buf[buf_n] = (char)cur_byte;
          if(cur_byte == END_MAGIC)
          {
            kspDebug->out("END MAGIC found!");
            recv_buf[buf_n] = '\0';
            end_magic_found = true;
          }
          buf_n+=1;
        }
        delay(25);
      }
      
      if(end_magic_found){
        return String(recv_buf);
      }
      return "";
    }

    
  private:
    KspDebug* kspDebug;
    KspSerial* kspSerial;
    
    
};
#endif
