#include <TinyWireM.h>
#include "ssd1327.h"

#define SSD1327_COMMAND_BYTE 0x00
#define SSD1327_DATA_BYTE 0xC0
#define MAX_PACKAGE_LENGTH 16
#define address 0x3C

static void dataStart(void);
static void stopTransmission(void);
static void sendCommand(uint8_t command1);
static void sendCommand(uint8_t command1, uint8_t command2);
static void sendCommand(uint8_t command1, uint8_t command2, uint8_t command3);

ssd1327::ssd1327() {
  TinyWireM.begin();
}

void ssd1327::startTransmission() {
	TinyWireM.beginTransmission(address);
}

void ssd1327::sendByte(uint8_t byte) {
	TinyWireM.write(byte);
}

void ssd1327::sendBytes(uint8_t *byte, uint8_t length) {
  startTransmission();
  
	for (uint8_t i = 0; i < length; i++) {
    sendByte(SSD1327_DATA_BYTE);
    sendByte(byte[i]);
	}
 
  stopTransmission();
}

void ssd1327::sendBytesAsOne(uint8_t *byte, uint8_t length) {
	TinyWireM.write(byte, length);
}

void ssd1327::stopTransmission(void) {
	TinyWireM.endTransmission();
}

void ssd1327::commandStart(void) {
	startTransmission();
	sendByte(SSD1327_COMMAND_BYTE);
}

void ssd1327::dataStart(void) {
	startTransmission();
	sendByte(SSD1327_DATA_BYTE);
}

void ssd1327::sendCommand(uint8_t command1) {
	commandStart();
	sendByte(command1);
	stopTransmission();
}

void ssd1327::sendCommand(uint8_t command1, uint8_t command2) {
	commandStart();
	sendByte(command1);
	sendByte(command2);
	stopTransmission();
}

void ssd1327::sendCommand(uint8_t command1, uint8_t command2, uint8_t command3) {
	commandStart();
	sendByte(command1);
	sendByte(command2);
	sendByte(command3);
	stopTransmission();
}

void ssd1327::draw(uint8_t value, bool useBuffer) {
  if (useBuffer) {
    if (combineBuffer) {
      drawBuffer[drawBufferCursor] += value;
      combineBuffer = false;
      drawBufferCursor++;
    }
    else {
      drawBuffer[drawBufferCursor] = value << 4;
      combineBuffer = true;
    }

    if (drawBufferCursor >= DRAW_BUFFER_SIZE) {
      sendBytes(drawBuffer, DRAW_BUFFER_SIZE);

      drawBufferCursor = 0;
      combineBuffer = false;
    }
  }
  else {
    dataStart();
    sendByte(value);
    stopTransmission();
    
    drawBufferCursor = 0;
    combineBuffer = false;
  }
}

void ssd1327::drawAllBuffer() {
  if (drawBufferCursor > 0) {
    sendBytes(drawBuffer, drawBufferCursor);

    drawBufferCursor = 0;
    combineBuffer = false;
  }
}

void ssd1327::on() {
  sendCommand(0xAF);
}

void ssd1327::off() {
  sendCommand(0xAE);
}

void ssd1327::drawCompressedBitmap(const uint8_t arr[], const uint8_t sequences[], uint16_t offset, uint8_t arrayLength, uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y) {
  sendCommand(0x15, x, x + sizeX / 2 - 1);
  sendCommand(0x75, y, y + sizeY - 1);

  for (uint16_t j = offset; j <= offset + arrayLength; j++) {
    uint16_t sequenceId = pgm_read_byte_near(arr + j);
    uint8_t repeatCount = pgm_read_byte_near(sequences + sequenceId);
    uint8_t value = pgm_read_byte_near(sequences + sequenceId + 1);

    for (uint16_t i = 0; i < repeatCount; i++) {
      draw(value, true);
    }
  }

  drawAllBuffer();
}

void ssd1327::drawBitmap(const uint8_t arr[], uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y) {
  uint16_t arrayLength = sizeX * sizeY;
  sendCommand(0x15, x, x + sizeX / 2 - 1);
  sendCommand(0x75, y, y + sizeY - 1);

  for (uint16_t j = 0; j < arrayLength; j++) {
    draw(pgm_read_byte_near(arr + j), true);
  }

  drawAllBuffer();
}

void ssd1327::clear(uint8_t color) {
	sendCommand(0x15, 0x00, column);//0x3F 0x2D
	sendCommand(0x75, 0x00, row);
  
	for (uint8_t x = 0; x < 128; x++){
    for (uint8_t y = 0; y < 112; y++) {
      draw(color, true);
    }
	}

 drawAllBuffer();
}

void ssd1327::clear(uint8_t color, uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y) {
  uint16_t arrayLength = sizeX * sizeY;
  sendCommand(0x15, x, x + sizeX / 2 - 1);
  sendCommand(0x75, y, y + sizeY - 1);

  for (uint16_t j = 0; j < arrayLength; j++) {
    draw(color, true);
  }

  drawAllBuffer();
}

void ssd1327::clearBorder(uint8_t color, uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y) {
  sendCommand(0x15, x, x + sizeX / 2 - 1);
  sendCommand(0x75, y, y);

  for (uint16_t j = 0; j < sizeX; j++) {
    draw(color, true);
  }

  sendCommand(0x15, x, x);
  sendCommand(0x75, y, y + sizeY - 1);

  for (uint16_t j = 0; j < sizeY; j++) {
    draw(color, true);
  }

  sendCommand(0x15, x, x);
  sendCommand(0x75, y + sizeY - 1, y + sizeY - 1);

  for (uint16_t j = 0; j < sizeY; j++) {
    draw(color, true);
  }
  
  sendCommand(0x15, x + sizeX / 2 - 1, x + sizeX / 2 - 1);
  sendCommand(0x75, y, y);

  for (uint16_t j = 0; j < sizeX; j++) {
    draw(color, true);
  }

  drawAllBuffer();
}

void ssd1327::remap(uint8_t value) {
  sendCommand(0xA0, value);
}

void ssd1327::remap(uint8_t value1, uint8_t value2) {
  sendCommand(0xA0, value1, value2);
}

void ssd1327::multiplex(uint8_t value) {
  sendCommand(0xA8, value);
}
