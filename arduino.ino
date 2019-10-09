/*
   Arduino main file

   FHICT @2019
   Kacper Serewiś PD06T15

   Programmed for: Arduino UNO R3

   Versie: 1.1
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



void printBootscreen()
{
  String boot_msg_1 = "--- Inklok system Alpha ---";
  String boot_msg_2 = "[*] Built on: " + String(__DATE__) + " " + String(__TIME__);
  String boot_msg_3 = "[*] FREE RAM: " + String(kspMonitor.freeRam());
  String boot_msg_4 = "--- By Kacper Serewis ---";

  kspDebug.set_color_to(0);
  kspDebug.plain_out(boot_msg_1);
  kspDebug.plain_out(boot_msg_2);
  kspDebug.plain_out(boot_msg_3);
  kspDebug.plain_out(boot_msg_4);
  kspDebug.set_color_to(-1);
  delay(2000);

}

void setup()
{
  kspDebug.init();
  printBootscreen();

  kspMonitor.init();
  kspCommunication.init();

  kspDesfire.init();
  bool rc522_connection = kspDesfire.kspConnectionCheck();

  kspMatrix.init();

  kspDebug.out("Waiting for server!");
  bool server_start_msg = false; //change to false
  while (!server_start_msg)
  {
    String command = kspCommunication.kspReceiveCommand();
    if (command == "server_ready")
    {
      server_start_msg = true;
    }
    kspMatrix.fill_random();
    delay(250);
  }

  kspCommunication.kspSendCommand("client_ready");

}





void loop()
{

  kspMatrix.sprite_wait();
  if (kspDesfire._0_cardAvailable())
  {
    kspDebug.out("Card Available!");
    kspDebug.out("Setup card connection!");
    if (kspDesfire._1_setupCardConnection())
    {
      kspDebug.out("Get NAME");
      String _name = kspDesfire._2_getName();
      kspDebug.out(_name);

      kspDebug.out("Get SCHOOL");
      String _school = kspDesfire._2_getSchool();
      kspDebug.out(_school);

      kspDebug.out("GET UID");
      String _uid = kspDesfire._3_getUID();
      kspDebug.out(_uid);

      kspMatrix.sprite_ok();
      kspCommunication.kspSendCommand("begin_auth");
      kspCommunication.kspSendCommand("s_name");
      kspCommunication.kspSendCommand(_name);
      kspCommunication.kspSendCommand("e_name");
      //kspCommunication.kspSendCommand(_school);
      
      kspCommunication.kspSendCommand("s_uid");
      kspCommunication.kspSendCommand(_uid);
      kspCommunication.kspSendCommand("e_uid");
      
      kspCommunication.kspSendCommand("end_auth");
      delay(1000);

    }
    else
    {
      kspCommunication.kspSendCommand("system_panic");
      kspMonitor.reset();
    }
  }
}
