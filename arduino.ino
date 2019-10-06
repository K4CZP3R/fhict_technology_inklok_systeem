/*
 * Arduino main file
 * 
 * FHICT @2019
 * Kacper Serewiś PD06T15
 * 
 * Programmed for: Arduino UNO R3
 * 
 * Versie: 1.0
 */

#include "KspDebug.cpp"
KspDebug kspDebug(true, "main");

#include "KspCommunication.cpp"
KspCommunication kspCommunication;

#include "KspDesfire.cpp"
KspDesfire kspDesfire;

#include "KspMatrix.cpp"
KspMatrix kspMatrix;

#include "KspMonitor.cpp"
KspMonitor kspMonitor;

// define here
char _scan_name[32];
char _scan_school[64];
char _serial_data[64];
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
void printBootscreen()
{
  const char* separator =  "=";
  const char* small_separator = "-";

  const char* boot_msg_1 = "Inklok system Alpha";
  const char* boot_msg_2 = "Built on:";
  const char* boot_msg_3 = __DATE__;
  const char* boot_msg_4 = __TIME__;
  const char* boot_msg_5 = "By Kacper Serewis";


  int msg_len = static_cast<int>(strlen(boot_msg_1));
  int margin = 4;

  int separator_length = margin + msg_len + margin;
  // ====
  kspDebug.set_color_to(3);
  for (int i = 0; i < separator_length; i++) {
    kspDebug.plain_out(separator, false);
  }
  kspDebug.plain_out("",true);
  
  
  //    msg1
  
  for(int i =0; i< margin; i++){
    kspDebug.plain_out(" ", false);
  }
  kspDebug.set_decor_to(2);
  kspDebug.plain_out(boot_msg_1);
  kspDebug.set_decor_to(-1);

  // ----
  kspDebug.set_color_to(3);
  for(int i =0; i < separator_length; i++) {
    kspDebug.plain_out(small_separator, false);
  }
  kspDebug.plain_out("",true);
  kspDebug.set_color_to(-1);

  //    msg2
  for(int i = 0; i< margin; i++)
  {
    kspDebug.plain_out(" ", false);
  }
  kspDebug.plain_out(boot_msg_2);

  //    msg3 msg4

  for(int i = 0; i< margin; i++)
  {
    kspDebug.plain_out(" ", false);
  }
  kspDebug.set_decor_to(2);
  kspDebug.plain_out(boot_msg_3, false);
  kspDebug.plain_out(" ",false);
  kspDebug.plain_out(boot_msg_4, true);
  kspDebug.set_decor_to(-1);

  //    msg5
  for(int i = 0; i< margin; i++)
  {
    kspDebug.plain_out(" ", false);
  }
  kspDebug.plain_out(boot_msg_5);

  // ====
  kspDebug.set_color_to(3);
  for (int i = 0; i < separator_length; i++) {
    kspDebug.plain_out(separator, false);
  }
  kspDebug.plain_out("",true);
  kspDebug.set_color_to(-1);

  for(int i =0; i<3; i++){
    kspDebug.plain_out("");
  }
  delay(1000);
}

void setup()
{
  kspDebug.init();
  printBootscreen();
  kspMonitor.init();


  kspCommunication.init();
  kspDesfire.init();
  bool rc522_connection = kspDesfire.kspConnectionCheck();


  kspDesfire.powerOff();
  kspMatrix.init();

  kspDebug.out("Waiting for server!");
  bool server_start_msg = false;
  while (!server_start_msg)
  {
     kspDebug.out(freeRam());
    kspCommunication.kspReceiveCommand();
    memcpy(_serial_data, kspCommunication.content_buf, sizeof(_serial_data)); //save data to main buf.
    if (strcmp(_serial_data, "server_ready") == 0)
    {
      server_start_msg = true;
    }
    wait_for_com_animation();
    delay(250);
  }


  kspCommunication.kspSendCommand("client_ready");

}

int wait_for_com_current_frame = 0;
int wait_for_com_last_frame = 9 - 1;
void wait_for_com_animation()
{
  kspMatrix.a_com_wait(wait_for_com_current_frame);
  wait_for_com_current_frame += 1;
  if (wait_for_com_current_frame > wait_for_com_last_frame)
  {
    wait_for_com_current_frame = 0;
  }
}

unsigned long t_wait_anim_last_time = 0;
int t_wait_anim_trigger_time = 500;

int t_wait_anim_current_frame = 0;
int t_wait_anim_max_frame = 2;
void timer_wait_animation()
{
  if (millis() - t_wait_anim_last_time > t_wait_anim_trigger_time)
  {
    t_wait_anim_last_time = millis();
    kspDesfire.powerOff();
    kspMatrix.a_touch(t_wait_anim_current_frame);
    kspDesfire.powerOn();
    t_wait_anim_current_frame += 1;
    if (t_wait_anim_current_frame > 2) {
      t_wait_anim_current_frame = 0;
    }

  }
}



void loop()
{

  kspDebug.out(freeRam());
  timer_wait_animation();



  if (kspDesfire._0_cardAvailable())
  {
    kspDesfire.powerOff();
    kspMatrix.a_wait();
    kspDesfire.powerOn();
    kspDebug.out("Card Available!");

    kspDebug.out("Setup card connection!");
    if (kspDesfire._1_setupCardConnection())
    {
      kspDebug.out("Get NAME");
      kspDesfire._2_getName();
      memcpy(_scan_name, kspDesfire.content_buf, sizeof(_scan_name));
      kspDebug.out(_scan_name);
      kspDebug.out(freeRam());
      
      kspDebug.out("Get SCHOOL");
      kspDesfire._2_getSchool();
      memcpy(_scan_school, kspDesfire.content_buf, sizeof(_scan_school));
      kspDebug.out(_scan_school);
      kspDebug.out(freeRam());
      

      kspCommunication.kspSendCommand("begin_auth");
      kspCommunication.kspSendCommand(_scan_name);
      kspCommunication.kspSendCommand(_scan_school);
      kspCommunication.kspSendCommand("end_auth");
      kspDebug.out(freeRam());
      kspDesfire.powerOff();
      kspMatrix.a_ok();
      kspDesfire.powerOn();
      delay(1000);
    }
    else
    {
      kspCommunication.kspSendCommand("system_panic");
      kspMonitor.reset();
    }
  }
  //kspDesfire.powerOff();
  //kspMatrix.a_bad();
  //kspDesfire.powerOn();

  //mfrc522->PICC_HaltA();
  //delay(1000);

}
