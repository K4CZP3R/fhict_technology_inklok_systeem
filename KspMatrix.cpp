/*
   KspMatrix

   Handles animations for 8x8 matrix panel.
   Special module created for this project.

   By Kacper Serewis
   1.1
*/
#ifndef KspMatrix_h
#define KspMatrix_h


#include "KspDebug.cpp"
#include <SPI.h>
#include "LedMatrix.h"


#define MATRIX_ID 0
#define MATRIX_INTENSITY 8

#define DIN 19
#define CLK 23
#define CS 17

class KspMatrix
{
  public:
    KspMatrix()
    {
      this->kspDebug = new KspDebug(true, "kspMatrix");
      this->ledMatrix = new LedMatrix(1, 23, 2, 19, 17);
    }

    void init()
    {
      kspDebug->init();
      kspDebug->out("Debug inited");

      ledMatrix->init();
      ledMatrix->clear();
      ledMatrix->setIntensity(15);
    }

    void fill_random()
    {
      for(int i=0; i<8; i++)
      {
        ledMatrix->setColumn(i, (byte)random(0,255));
      }
      ledMatrix->commit();
    }
    void sprite_wait()
    {
      byte sprite_wait[] {B00111000, B00000100, B00000100, B00001000, B00010000, B00010000, B00000000, B00010000};
      show_sprite(sprite_wait);
    }
    void sprite_bad()
    {
      byte sprite_bad[] = { B11000011, B11100111, B01100110, B00011000, B00011000, B01100110, B11100111, B11000011};
      show_sprite(sprite_bad);
    }
    void sprite_ok()
    {
      byte sprite_ok[] = { B00000000, B00000001, B00000010, B01000100, B00101000, B00010000, B00000000, B00000000};
      show_sprite(sprite_ok);
    }

  private:
    LedMatrix* ledMatrix;
    KspDebug* kspDebug;

    void show_sprite(byte sprite[])
    {
      for (int i = 0; i < 8; i++)
      {
        ledMatrix->setColumn(i, sprite[i]);
      }
      ledMatrix->commit();
    }
};

#endif
