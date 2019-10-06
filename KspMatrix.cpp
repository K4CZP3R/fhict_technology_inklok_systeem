/*
 * KspMatrix
 * 
 * Handles animations for 8x8 matrix panel.
 * Special module created for this project.
 * 
 * By Kacper Serewis
 * 1.0
 */
#ifndef KspMatrix_h
#define KspMatrix_h

#include "LedControl.h"
#include "KspDebug.cpp"
#include <SPI.h>

#define MATRIX_ID 0
#define MATRIX_INTENSITY 8


class KspMatrix
{
  public:
    KspMatrix()
    {
      this->kspDebug = new KspDebug(true, "kspMatrix");
      this->lc = new LedControl(12, 11, 6, 1);
    }

    void init()
    {
      kspDebug->init();
      kspDebug->out("Debug inited");

      lc->shutdown(MATRIX_ID, false);
      lc->setIntensity(MATRIX_ID, MATRIX_INTENSITY);
      lc->clearDisplay(MATRIX_ID);

      kspDebug->out("LedControl inited!");
    }
    void a_touch(int frame)
    {
      byte sprites[][8] =
      {
        { B00000000, B00000000, B00010000, B01101000, B01101000, B00010000, B00000000, B00000000}, //frame 0
        {B00000000, B00001000, B00000100, B01100100, B01100100, B00000100, B00001000, B00000000}, //frame 1
        {B00000100, B00000010, B00000010, B01100010, B01100010, B00000010, B00000010, B00000100} //frame 3
      };

      show_animation(sprites[frame]);
    }
    void a_ok()
    {
      byte sprite_ok[] = { B00000000, B00000001, B00000010, B01000100, B00101000, B00010000, B00000000, B00000000};

      show_animation(sprite_ok);
    }
    void a_bad()
    {
      byte sprite_bad[] = { B11000011, B11100111, B01100110, B00011000, B00011000, B01100110, B11100111, B11000011};
      show_animation(sprite_bad);
    }
    void a_wait()
    {
      byte sprite_wait[] = { B00111000, B00000100, B00000100, B00001000, B00010000, B00010000, B00000000, B00010000};
      show_animation(sprite_wait);
    }

    void a_com_wait(int frame)
    {
      byte sprites[][8] = 
      {
        {B00000000,B00011100,B00000010,B00000000,B00000000,B00000000,B00000000,B00000000},
        {B00000000,B00011100,B00000010,B00000000,B00000000,B00000000,B00000000,B00000000},
        {B00000000,B00000100,B00000010,B00000010,B00000010,B00000000,B00000000,B00000000},
        {B00000000,B00000000,B00000000,B00000010,B00000010,B00000010,B00000100,B00000000},
        {B00000000,B00000000,B00000000,B00000000,B00000000,B00000010,B00011100,B00000000},
        {B00000000,B00000000,B00000000,B00000000,B00000000,B01000000,B00111000,B00000000},
        {B00000000,B00000000,B00000000,B01000000,B01000000,B01000000,B00100000,B00000000},
        {B00000000,B00100000,B01000000,B01000000,B01000000,B00000000,B00000000,B00000000},
        {B00000000,B00111000,B01000000,B00000000,B00000000,B00000000,B00000000,B00000000},
      };

      show_animation(sprites[frame]);
    }


  private:
    LedControl* lc;
    KspDebug* kspDebug;

    void show_animation(byte anim[])
    {
      for (int i = 0; i < 8; i++)
      {
        lc->setRow(0, i, anim[i]);
      }
    }

};

#endif
