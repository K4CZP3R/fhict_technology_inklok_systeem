/*
 * KspDesfire
 * 
 * Custom wrapper for mfrc522 and desfire for it.
 * Created speccial for this project.
 * 
 * By Kacper Serewis
 * 1.0
 */

#ifndef KspDesfire_h
#define KspDesfire_h

#include <SPI.h>
#include <MFRC522.h>
#include <Desfire.h>

#define MFRC_RST 9
#define MFRC_SS 10

#include "KspDebug.cpp"

class KspDesfire
{
  public:
    char content_buf[130];
    KspDesfire()
    {
      this->kspDebug = new KspDebug(true, "KspDesfire");
      this->mfrc522 = new DESFire(MFRC_SS, MFRC_RST);
    }

    void init()
    {
      kspDebug->init();
      kspDebug->out("Debug inited!");

      kspDebug->out("init: SPI!");
      powerOn();
      kspDebug->out("pcd init");
      mfrc522->PCD_Init();
    }

    bool kspConnectionCheck() //do it in init()
    {
      byte b = mfrc522->PCD_ReadRegister(0x37 << 1); //version register
      kspDebug->out("RC522 Version:");
      kspDebug->outHex(b);

      if ((b == 0x00) || (b == 0xFF))
      {
        kspDebug->out("There is no connection with RC522!");
        return false;
      }
      return true;
    }



    bool _0_cardAvailable()
    {
      if (!newCardPresent()) return false;
      if (!isDesfireCard()) return false;
      return true;
    }

    bool _1_setupCardConnection()
    {
      setupDesfire();
      if (desfire_error_occured()) return false;
      requestATS();
      if (desfire_error_occured()) return false;
      selectPP();
      if (desfire_error_occured()) return false;

      getMasterKey();
      if (desfire_error_occured()) return false;

      DESFire::mifare_desfire_aid_t aid_fontys = getApp(0x00);
      if (desfire_error_occured()) return false;
      kspDebug->out("App fontys AID: ");
      for (int i = 0; i < 3; i++) {
        kspDebug->outHex(aid_fontys.data[i]);
      }

      selectApp(aid_fontys);
      if (desfire_error_occured()) return false;

      return true;
    }

    void _2_getName()
    {
      byte fileId = getFileID(0x00);
      if (desfire_error_occured()) return "";

      DESFire::mifare_desfire_file_settings_t settings = getFileSettings(fileId);
      if (desfire_error_occured()) return "";

      getFileData(fileId, settings);
      kspDebug->out(content_buf);
    }
    void _2_getSchool()
    {
      byte fileId = getFileID(0x01);
      if (desfire_error_occured()) return "";

      DESFire::mifare_desfire_file_settings_t settings = getFileSettings(fileId);
      if (desfire_error_occured()) return "";

      getFileData(fileId, settings);
      kspDebug->out(content_buf);
    }

    void powerOff()
    {
      //mfrc522->PCD_SoftPowerDown();
      SPI.end();
    }
    void powerOn()
    {
      //mfrc522->PCD_SoftPowerUp();
      SPI.begin();
    }



  private:
    DESFire* mfrc522;
    DESFire::mifare_desfire_tag tag;
    DESFire::StatusCode response;

    KspDebug* kspDebug;

    bool newCardPresent()
    {
      if (!mfrc522->PICC_IsNewCardPresent())
      {
        return false;
      }
      if (!mfrc522->PICC_ReadCardSerial())
      {
        return false;
      }
      return true;
    }

    bool isDesfireCard()
    {
      return mfrc522->uid.sak == 0x20;
    }
    void requestATS() {
      byte ats[16];
      byte atsLength = 16;
      response.mfrc522 = mfrc522->PICC_RequestATS(ats, &atsLength);
    }
    void selectPP()
    {
      response.mfrc522 = mfrc522->PICC_ProtocolAndParameterSelection(0x00, 0x11);
    }

    bool desfire_error_occured() {
      if (!mfrc522->IsStatusCodeOK(response)) {
        kspDebug->out("=========");
        kspDebug->out("= ERROR =");
        kspDebug->out("=========");
        kspDebug->out(mfrc522->GetStatusCodeName(response));
        return true;
      }
      return false;
    }
    void setupDesfire() {
      tag.pcb = 0x0A;
      tag.cid = 0x00;
      memset(tag.selected_application, 0, 3);

      response.desfire = DESFire::MF_OPERATION_OK;
    }
    /*DESFire::MIFARE_DESFIRE_Version_t getVersion() { //dont use, dont need it
      DESFire::MIFARE_DESFIRE_Version_t desfireVersion;
      response = mfrc522->MIFARE_DESFIRE_GetVersion(&tag, &desfireVersion);
      kspDebug->out("Version major: ");
      kspDebug->outHex(desfireVersion.hardware.version_major);
      kspDebug->out("Storage size: ");
      kspDebug->outHex(desfireVersion.hardware.storage_size);
      return desfireVersion;
      }*/
    void selectApp(DESFire::mifare_desfire_aid_t aid) { //pass aid like this?!
      response = mfrc522->MIFARE_DESFIRE_SelectApplication(&tag, &aid);
    }
    void getMasterKey() {
      DESFire::mifare_desfire_aid_t aid;
      aid.data[0] = 0x00;
      aid.data[1] = 0x00;
      aid.data[2] = 0x00;

      selectApp(aid);
      if (desfire_error_occured()) return;

      byte keySettings;
      byte keyCount = 0;
      byte keyVersion;
      response = mfrc522->MIFARE_DESFIRE_GetKeySettings(&tag, &keySettings, &keyCount);
      if (!desfire_error_occured()) {
        if (keyCount > 0) {
          for (byte ixKey = 0; ixKey < keyCount; ixKey++) {
            response = mfrc522->MIFARE_DESFIRE_GetKeyVersion(&tag, ixKey, &keyVersion);

            //print??
          }
        }
      }

    }
    DESFire::mifare_desfire_aid_t getApp(int index_id) {
      DESFire::mifare_desfire_aid_t aids[2]; //was 16
      byte applicationCount = 0;
      response = mfrc522->MIFARE_DESFIRE_GetApplicationIds(&tag, aids, &applicationCount);
      return aids[index_id];
    }
    byte getFileID(int index_id) {
      byte files[2]; //was 64
      byte filesCount = 0;
      response = mfrc522->MIFARE_DESFIRE_GetFileIDs(&tag, files, &filesCount);
      return files[index_id];

    }
    DESFire::mifare_desfire_file_settings_t getFileSettings(byte file) {
      kspDebug->outDescHex(">> File", file);
      DESFire::mifare_desfire_file_settings_t fileSettings;

      response = mfrc522->MIFARE_DESFIRE_GetFileSettings(&tag, &(file), &fileSettings);

      kspDebug->outDescHex("file_type", fileSettings.file_type);
      kspDebug->outDescHex("file_size", fileSettings.settings.standard_file.file_size);
      return fileSettings;
    }

    void getFileData(byte file, DESFire::mifare_desfire_file_settings_t fileSettings) {
      //String getFileData(byte file, DESFire::mifare_desfire_file_settings_t fileSettings) {
      byte fileContent[fileSettings.settings.standard_file.file_size];
      size_t fileContentLength = fileSettings.settings.standard_file.file_size;
      response = mfrc522->MIFARE_DESFIRE_ReadData(&tag, file, 0, fileContentLength, fileContent, &fileContentLength);

      char buf[fileContentLength];
      for (unsigned int iByte = 0; iByte < fileContentLength; iByte++) {
        byte current_byte = fileContent[iByte];

        if (current_byte == 0x00) {
          buf[iByte] = '\0';
          break;
        }
        buf[iByte] = char(current_byte);
      }



      kspDebug->out(buf);
      memcpy(content_buf, buf, sizeof(content_buf));
    }


};

#endif
