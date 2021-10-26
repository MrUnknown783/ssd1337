#include <TinyWireM.h>
#include "ssd1327.h"
//0 01000000 00001100
#define F_CPU 8000000UL
#define SSD1327_COMMAND_BYTE 0x00
#define SSD1327_DATA_BYTE 0xC0
#define MAX_PACKAGE_LENGTH 16
#define address 0x3C

static void dataStart(void);
static void stopTransmission(void);
static void sendCommand(uint8_t command1);
static void sendCommand(uint8_t command1, uint8_t command2);
static void sendCommand(uint8_t command1, uint8_t command2, uint8_t command3);

static void startTransmission() {
	TinyWireM.beginTransmission(address);
}

static bool sendByte(uint8_t byte) {
	return TinyWireM.write(byte);
}

static void sendBytes(uint8_t *byte, uint8_t length) {
  startTransmission();
	for (uint8_t i = 0; i < length; i++) {
		/*uint8_t left = i + MAX_PACKAGE_LENGTH >= length ? length - i : MAX_PACKAGE_LENGTH;
		
		dataStart();
		for (uint8_t j = 0; j < left; j++) {
			sendByte(byte[i + j]);
		}
		stopTransmission();*/
    sendByte(SSD1327_DATA_BYTE);
    sendByte(byte[i]);
	}
  stopTransmission();
}

static bool sendBytesAsOne(uint8_t *byte, uint8_t length) {
	return TinyWireM.write(byte, length);
}

static void stopTransmission(void) {
	TinyWireM.endTransmission();
}

static void commandStart(void) {
	startTransmission();
	sendByte(SSD1327_COMMAND_BYTE);
}

static void dataStart(void) {
	startTransmission();
	sendByte(SSD1327_DATA_BYTE);
}

static void sendCommand(uint8_t command1) {
	commandStart();
	sendByte(command1);
	stopTransmission();
}

static void sendCommand(uint8_t command1, uint8_t command2) {
	commandStart();
	sendByte(command1);
	sendByte(command2);
	stopTransmission();
}

static void sendCommand(uint8_t command1, uint8_t command2, uint8_t command3) {
	commandStart();
	sendByte(command1);
	sendByte(command2);
	sendByte(command3);
	stopTransmission();
}

#define DRAW_BUFFER_SIZE 8
static uint8_t drawBuffer[DRAW_BUFFER_SIZE];
static uint8_t drawBufferCursor;
static bool combineBuffer = false;

static void draw(uint8_t value, bool useBuffer) {
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
      /*for (uint8_t i = 0; i < DRAW_BUFFER_SIZE; i++) {
        dataStart();
        sendByte(drawBuffer[i]);
        stopTransmission();
      }*/

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

static void _drawBuffer() {
  if (drawBufferCursor > 0) {
    sendBytes(drawBuffer, drawBufferCursor);

    drawBufferCursor = 0;
    combineBuffer = false;
  }
}

ssd1327::ssd1327() {
	TinyWireM.begin();
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
  delay(10);

  /*for (uint16_t j = 1; j <= length; j++) {
    uint16_t data = pgm_read_byte_near(arr + j);

    uint8_t value = data << 4;
    value = value >> 4;
    
    uint16_t repeatCount = (data - value) >> 4;

    for (uint16_t i = 0; i < repeatCount; i++) {
      draw(value, true);
    }
  }*/

  for (uint16_t j = offset; j <= offset + arrayLength; j++) {
    uint16_t sequenceId = pgm_read_byte_near(arr + j);
    uint8_t repeatCount = pgm_read_byte_near(sequences + sequenceId);
    uint8_t value = pgm_read_byte_near(sequences + sequenceId + 1);

    for (uint16_t i = 0; i < repeatCount; i++) {
      draw(value, true);
    }
  }

  _drawBuffer();
}

void ssd1327::drawBitmap(const uint8_t arr[], uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y) {
  uint16_t arrayLength = sizeX * sizeY;
  sendCommand(0x15, x, x + sizeX / 2 - 1);
  sendCommand(0x75, y, y + sizeY - 1);
  delay(10);

  for (uint16_t j = 0; j < arrayLength; j++) {
    draw(pgm_read_byte_near(arr + j), true);
  }

  _drawBuffer();
}

void ssd1327::clear(uint8_t color) {
	sendCommand(0x15, 0x00, 0x3F);//0x3F 0x2D
	sendCommand(0x75, 0x00, 0x7F);
  
	for (uint8_t x = 0; x < 128; x++){
    for (uint8_t y = 0; y < 112; y++) {
      draw(color, true);
    }
	}

 _drawBuffer();
}

void ssd1327::clear(uint8_t color, uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y) {
  uint16_t arrayLength = sizeX * sizeY;
  sendCommand(0x15, x, x + sizeX / 2 - 1);
  sendCommand(0x75, y, y + sizeY - 1);
  delay(10);

  for (uint16_t j = 0; j < arrayLength; j++) {
    draw(color, true);
  }

  _drawBuffer();
}

void ssd1327::clearBorder(uint8_t color, uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y) {
  sendCommand(0x15, x, x + sizeX / 2 - 1);
  sendCommand(0x75, y, y);
  delay(10);

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

  _drawBuffer();
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
