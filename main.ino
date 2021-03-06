//#include <SoftwareSerial.h>
//#include "src/libs/TinyWireM/TinyWireM.h"
#include "src/libs/ssd1337/ssd1327.h"

//SoftwareSerial serial(4, 3);

#define SSD1327_COMMAND_BYTE 0x00
#define SSD1327_DATA_BYTE 0x40
#define MAX_PACKAGE_LENGTH 16
#define address 0x3C

ssd1327 _display;

void pr(char* data) {
  //serial.println(data);
}

void setup() {
  //serial.begin(9600);

  _display.sendCommand(0xFD, 0x01, 0x12);                               // Unlock OLED
  _display.sendCommand(0xAE, 0x00);                                     // OFF
  //sendCommand(0xA8, 0x7F);                                            // multiplex 128
  //sendCommand(0xA1, 0x01, 0x00);                                      // Start line
  //sendCommand(0xA2, 0x01, 0x00);                                      // Display offset
  _display.remap(0x51);
  //_display.sendCommand(0xA0, 0x01, 0x40 | 0x10 | 0x00 | (0x02 | 0x01));
  _display.sendCommand(0xAB, 0x01, 0x01);                               // VDD internal
  //_display.sendCommand(0x81, 0x01, 0x70);                             // CONTRAST
  /*_display.sendCommand(0xB1, 0x01, 0x55);                             // PHASE 0x51
  _display.sendCommand(0xB3, 0x01, 0x01);                               // CLK*/
  _display.sendCommand(0xBC, 0x01, 0x08);                               // PRECHARGE
  _display.sendCommand(0xBE, 0x01, 0x07);                               // VCOMH voltage
  _display.sendCommand(0xB6, 0x01, 0x01);                               // Second pre-charge
  _display.sendCommand(0xA4, 0x00);                                     // NORMAL
  _display.sendCommand(0x2E, 0x00);                                     // Deactivate scroll

  _display.on();
  delay(500);
}

const PROGMEM uint8_t heartLength[] = { 61 };
const PROGMEM uint8_t heartPixels[] = { 0, 2, 4, 2, 6, 8, 10, 8, 12, 14, 16, 14, 0, 18, 20, 18, 22, 24, 20, 24, 26, 28, 26, 24, 20, 24, 20, 24, 22, 30, 16, 30, 0, 32, 12, 34, 6, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 2, 68, 70, 72, 74, 76, 78, 50 };
const PROGMEM uint8_t heartSequenceDictionary[] = { 5, 0, 8, 15, 10, 0, 9, 0, 11, 15, 6, 0, 7, 0, 13, 15, 4, 0, 15, 15, 2, 0, 3, 0, 34, 15, 1, 0, 252, 15, 32, 15, 30, 15, 28, 15, 26, 15, 11, 0, 24, 15, 13, 0, 22, 15, 15, 0, 20, 15, 17, 0, 18, 15, 19, 0, 16, 15, 22, 0, 12, 15, 25, 0, 10, 15, 27, 0, 29, 0, 6, 15, 31, 0, 4, 15, 33, 0, 2, 15 };

const uint8_t heartWidth = 36;
const uint8_t heartHeight = 32;

const PROGMEM uint8_t framesLength[]{55, 51, 59, 59, 63, 63, 55, 59, 55, 53, 55, 57, 57, 55, 57, 59, 65, 63, 63, 61, 59, 61, 59, 55, 47, 49, 59};
const PROGMEM uint8_t frames[]{0, 2, 4, 6, 8, 10, 12, 14, 16, 10, 18, 14, 16, 10, 16, 10, 20, 2, 22, 10, 20, 24, 26, 28, 30, 14, 20, 28, 32, 34, 36, 38, 36, 38, 36, 38, 36, 38, 32, 34, 32, 38, 40, 42, 32, 34, 32, 34, 40, 42, 40, 42, 44, 46, 48, 50, 10, 52, 2, 54, 6, 56, 58, 60, 14, 20, 14, 62, 14, 20, 6, 26, 64, 66, 68, 30, 70, 32, 70, 32, 34, 32, 38, 36, 38, 36, 38, 32, 38, 36, 38, 36, 38, 36, 38, 36, 38, 36, 34, 36, 38, 32, 34, 32, 70, 72, 74, 2, 54, 6, 76, 10, 78, 14, 80, 14, 22, 14, 82, 2, 84, 2, 20, 24, 30, 70, 32, 34, 36, 38, 36, 38, 36, 38, 36, 38, 36, 42, 40, 42, 36, 86, 20, 88, 36, 86, 82, 88, 40, 90, 20, 88, 40, 38, 36, 38, 36, 34, 32, 70, 40, 38, 32, 38, 72, 92, 2, 54, 2, 4, 24, 12, 14, 94, 58, 84, 2, 82, 6, 30, 24, 20, 88, 32, 38, 36, 38, 36, 96, 20, 6, 36, 90, 82, 6, 36, 96, 20, 6, 36, 38, 36, 42, 40, 42, 40, 46, 44, 46, 98, 100, 36, 38, 36, 34, 32, 70, 36, 70, 102, 34, 66, 68, 72, 92, 2, 54, 6, 76, 10, 82, 10, 12, 14, 94, 10, 26, 2, 82, 2, 66, 24, 82, 88, 36, 38, 36, 64, 82, 58, 40, 104, 20, 58, 40, 104, 20, 58, 40, 42, 40, 42, 40, 42, 44, 100, 106, 2, 20, 70, 108, 110, 112, 114, 106, 116, 106, 116, 40, 38, 32, 118, 30, 118, 30, 118, 72, 120, 14, 52, 2, 4, 6, 8, 10, 62, 10, 48, 10, 76, 14, 122, 2, 82, 14, 124, 88, 82, 2, 30, 86, 20, 24, 36, 42, 40, 118, 20, 10, 40, 118, 20, 10, 44, 46, 44, 46, 44, 46, 98, 100, 106, 114, 108, 110, 108, 110, 112, 110, 108, 116, 36, 38, 36, 34, 32, 118, 30, 118, 126, 74, 2, 54, 2, 8, 10, 16, 10, 76, 14, 128, 2, 76, 24, 82, 58, 30, 90, 20, 6, 32, 38, 40, 42, 40, 42, 40, 42, 40, 42, 40, 34, 36, 38, 40, 100, 98, 6, 20, 68, 106, 116, 106, 114, 108, 114, 98, 100, 40, 38, 36, 38, 36, 118, 130, 132, 2, 54, 2, 8, 10, 12, 10, 134, 10, 54, 2, 20, 14, 84, 24, 20, 2, 30, 86, 20, 24, 36, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 68, 20, 14, 44, 118, 20, 14, 44, 46, 44, 10, 20, 34, 98, 100, 98, 100, 98, 46, 40, 42, 36, 118, 112, 136, 6, 4, 6, 8, 10, 16, 10, 76, 10, 138, 10, 140, 14, 54, 96, 142, 68, 102, 86, 20, 24, 36, 38, 36, 42, 40, 42, 40, 42, 40, 28, 20, 6, 44, 64, 20, 6, 40, 42, 40, 42, 44, 46, 44, 100, 44, 42, 36, 38, 36, 34, 32, 70, 130, 144, 14, 52, 6, 4, 6, 16, 10, 146, 14, 52, 58, 54, 88, 148, 104, 66, 118, 32, 38, 40, 42, 40, 42, 40, 42, 40, 42, 40, 64, 20, 2, 40, 64, 20, 2, 40, 64, 20, 2, 36, 42, 36, 42, 36, 38, 36, 34, 44, 42, 40, 38, 130, 150, 58, 52, 2, 54, 6, 16, 10, 76, 10, 82, 10, 152, 10, 16, 10, 52, 58, 76, 90, 84, 68, 102, 24, 48, 24, 36, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 118, 20, 10, 40, 42, 40, 46, 44, 100, 106, 116, 106, 100, 44, 46, 154, 156, 2, 54, 6, 140, 14, 16, 10, 16, 10, 158, 10, 16, 10, 16, 10, 76, 24, 22, 6, 20, 10, 142, 86, 20, 58, 30, 88, 48, 24, 36, 38, 36, 42, 40, 42, 40, 42, 44, 46, 44, 86, 20, 90, 44, 46, 44, 46, 44, 114, 112, 114, 108, 114, 106, 114, 126, 156, 58, 52, 2, 54, 6, 16, 10, 152, 10, 16, 10, 16, 10, 54, 2, 148, 24, 20, 10, 142, 88, 82, 14, 66, 24, 72, 2, 32, 96, 20, 6, 36, 42, 40, 42, 40, 42, 40, 42, 40, 42, 44, 46, 44, 46, 44, 46, 98, 100, 82, 10, 154, 160, 112, 160, 72, 0, 10, 52, 6, 4, 6, 16, 10, 162, 10, 164, 10, 16, 10, 54, 2, 22, 90, 84, 24, 20, 58, 66, 88, 48, 2, 102, 90, 20, 24, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 44, 46, 44, 46, 44, 46, 44, 46, 44, 46, 44, 46, 108, 136, 14, 54, 6, 76, 24, 76, 10, 16, 10, 16, 58, 166, 2, 76, 24, 62, 90, 84, 6, 20, 6, 30, 24, 82, 24, 32, 38, 36, 38, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 44, 100, 98, 46, 44, 58, 20, 68, 44, 42, 36, 34, 98, 168, 58, 4, 6, 76, 6, 4, 14, 140, 14, 140, 58, 166, 14, 52, 88, 148, 2, 20, 2, 142, 6, 20, 6, 30, 24, 20, 88, 40, 42, 36, 38, 36, 38, 36, 38, 36, 38, 40, 46, 44, 46, 44, 46, 44, 46, 44, 46, 44, 42, 44, 42, 36, 70, 102, 70, 30, 70, 44, 170, 14, 54, 6, 76, 24, 62, 58, 52, 58, 52, 58, 52, 14, 172, 14, 76, 14, 48, 10, 26, 6, 82, 2, 66, 24, 82, 6, 32, 88, 48, 6, 36, 38, 36, 38, 36, 38, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 44, 42, 40, 46, 44, 100, 44, 42, 40, 34, 102, 70, 30, 118, 30, 118, 40, 174, 58, 54, 6, 76, 6, 76, 58, 52, 58, 52, 58, 176, 10, 52, 2, 48, 10, 142, 24, 48, 58, 102, 86, 48, 2, 32, 38, 36, 38, 40, 42, 40, 42, 44, 46, 44, 46, 44, 46, 44, 46, 44, 46, 44, 100, 98, 160, 130, 178, 154, 180, 130, 110, 106, 68, 82, 14, 44, 68, 66, 68, 40, 170, 14, 54, 6, 76, 24, 62, 58, 52, 58, 52, 58, 176, 10, 140, 14, 4, 24, 82, 58, 30, 88, 72, 2, 32, 88, 48, 6, 36, 42, 40, 42, 40, 42, 44, 46, 44, 46, 44, 46, 44, 46, 44, 46, 44, 46, 44, 100, 98, 116, 98, 110, 112, 178, 108, 110, 112, 160, 130, 118, 48, 10, 154, 182, 6, 76, 24, 62, 24, 76, 58, 52, 58, 52, 14, 184, 14, 4, 6, 48, 10, 66, 86, 48, 58, 102, 88, 72, 2, 32, 38, 40, 42, 40, 42, 40, 42, 40, 42, 44, 46, 44, 46, 44, 46, 44, 100, 98, 100, 98, 100, 98, 160, 130, 178, 112, 114, 98, 100, 98, 68, 20, 58, 130, 186, 58, 54, 2, 76, 24, 76, 14, 140, 14, 140, 10, 188, 14, 48, 10, 26, 2, 48, 58, 66, 24, 48, 6, 36, 86, 48, 6, 36, 38, 36, 38, 36, 38, 36, 38, 40, 42, 40, 42, 40, 42, 40, 42, 40, 38, 40, 42, 40, 46, 40, 46, 44, 46, 40, 38, 32, 118, 40, 168, 10, 52, 2, 54, 6, 76, 10, 78, 58, 190, 10, 20, 14, 22, 58, 20, 58, 84, 2, 20, 24, 30, 24, 48, 24, 44, 86, 48, 24, 40, 42, 36, 38, 36, 38, 36, 42, 44, 46, 44, 46, 44, 68, 20, 58, 44, 46, 44, 46, 44, 46, 44, 42, 40, 38, 36, 70, 30, 118, 40, 192, 10, 52, 6, 4, 6, 194, 6, 196, 10, 82, 10, 4, 10, 20, 10, 76, 14, 20, 2, 84, 104, 30, 2, 72, 24, 32, 86, 20, 24, 36, 38, 36, 38, 36, 42, 40, 42, 40, 42, 40, 42, 40, 104, 20, 58, 40, 46, 44, 46, 44, 42, 40, 42, 32, 118, 66, 64, 44, 198, 14, 54, 6, 4, 24, 16, 10, 16, 10, 76, 6, 200, 10, 16, 10, 4, 6, 62, 28, 142, 68, 102, 34, 40, 90, 20, 88, 40, 42, 40, 42, 40, 42, 40, 46, 44, 46, 44, 46, 44, 46, 32, 38, 32, 34, 98, 10, 20, 68, 36, 46, 44, 46, 130, 202, 58, 54, 6, 54, 2, 190, 10, 16, 10, 54, 2, 54, 6, 62, 58, 20, 58, 84, 68, 36, 42, 40, 42, 36, 42, 44, 46, 44, 46, 44, 46, 44, 46, 44, 100, 98, 100, 98, 116, 106, 116, 106, 116, 106, 114, 72, 204, 14, 54, 6, 4, 6, 194, 58, 206, 10, 16, 10, 16, 10, 52, 2, 76, 86, 26, 6, 82, 58, 66, 70, 32, 38, 36, 38, 36, 38, 40, 46, 44, 46, 44, 46, 44, 46, 44, 100, 98, 100, 106, 116, 106, 116, 106, 116, 126, 208, 10, 52, 6, 4, 6, 194, 10, 200, 14, 140, 14, 140, 14, 140, 14, 20, 10, 148, 6, 20, 2, 66, 58, 126, 24, 32, 2, 126, 24, 36, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 40, 42, 32, 38, 98, 10, 20, 34, 98, 10, 20, 34, 98, 100, 98, 100, 72};
const PROGMEM uint8_t sequenceDictionary[] = {219, 0, 5, 13, 28, 0, 6, 13, 33, 0, 2, 13, 61, 0, 3, 13, 32, 0, 100, 0, 2, 0, 25, 0, 7, 13, 23, 0, 12, 13, 18, 0, 16, 0, 18, 13, 15, 0, 19, 13, 14, 0, 20, 13, 13, 0, 21, 13, 4, 0, 154, 0, 30, 0, 29, 0, 95, 0, 4, 13, 167, 0, 26, 0, 13, 13, 19, 0, 15, 13, 17, 13, 5, 0, 119, 0, 27, 0, 66, 0, 207, 0, 3, 0, 22, 0, 9, 13, 8, 13, 10, 13, 86, 0, 240, 0, 11, 13, 12, 0, 22, 13, 17, 0, 14, 13, 11, 0, 10, 0, 25, 13, 9, 0, 24, 13, 23, 13, 16, 13, 87, 0, 231, 0, 20, 0, 6, 0, 232, 0, 8, 0, 152, 0, 203, 0, 184, 0, 170, 0, 31, 0, 21, 0, 185, 0, 199, 0, 24, 0, 218, 0, 132, 0, 7, 0, 252, 0, 98, 0, 26, 13, 62, 0, 103, 0, 134, 0, 149, 0, 82, 0, 136, 0, 81, 0, 165, 0, 27, 13, 28, 13, 114, 0, 166, 0, 115, 0, 201, 0, 168, 0, 183, 0, 96, 0, 130, 0, 217, 0, 101, 0, 251, 0, 253, 0, 67, 0, 220, 0};

const uint8_t animationFrames = 27;
const uint8_t animationWidth = 34;
const uint8_t animationHeight = 33;

void loop() {
  _display.clear(0);
  //_display.clearBorder(15, 0, 2, heartWidth, heartHeight);

  for (uint8_t i = 0; i < 40; i++) {
    if (i > 0) {
      delay(10);
      _display.clearBorder(0, i - 1, i + 1, heartWidth, heartHeight);
    }
    
    //_display.drawBitmap(heart, i, i + 2, heartWidth, heartHeight);
    _display.drawCompressedBitmap(heartPixels, heartSequenceDictionary, 0, pgm_read_byte_near(heartLength), i, i + 2, heartWidth, heartHeight);
  }
  
  _display.clear(0, 39, 41, heartWidth, heartHeight);

  for (uint8_t j = 0; j < animationFrames; j++) {//18 19 20  21  23 25
    uint16_t offset = 0;
    
    for (uint8_t i = 0; i < animationFrames; i++) {
      if (i > 0) {
        //_display.clear(0, animationWidth, animationHeight, 40, 40);
      }
      
      _display.drawCompressedBitmap(frames, sequenceDictionary, offset, pgm_read_byte_near(framesLength + i), 40, 40, animationWidth, animationHeight);
      //_display.drawBitmapUsingPalette(frames, palette, offset, repeats, pgm_read_byte_near(framesLength + i), animationWidth, animationHeight, 40, 40);
      offset += pgm_read_byte_near(framesLength + i);
      delay(30);
    }
  }
  
  delay(20000);
}