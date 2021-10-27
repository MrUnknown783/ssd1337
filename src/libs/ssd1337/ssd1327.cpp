#include "../TinyWireM_Local/TinyWireM.h"
#include "ssd1327_commands.h"
#include "ssd1327.h"

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

void ssd1327::setCursor(uint8_t x, uint8_t y) {
  sendCommand(SSD1327_SET_COLUMN_COMMAND, x, x);
  sendCommand(SSD1327_SET_ROW_COMMAND, y, y);
}

void ssd1327::setDrawRange(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  uint8_t xEnd = 0;
  uint8_t yEnd = y + height - 1;

  if (width == 0) {
    xEnd = x;
  }
  else if (width % 2 != 0) {
    xEnd = x + (width - 1) / 2;
  }
  else {
    xEnd = x + width / 2 - 1;
  }
  
  sendCommand(SSD1327_SET_COLUMN_COMMAND, x, xEnd);
  sendCommand(SSD1327_SET_ROW_COMMAND, y, yEnd);
}

void ssd1327::draw(uint8_t value, bool useBuffer, bool skip) {
  if (useBuffer) {
    if (skip) {
      drawBufferCursor++;
      combineBuffer = false;
    }
    else  {
      draw(value, useBuffer);
    }
  }
  else {
    draw(value, useBuffer);
  }
}

void ssd1327::draw(uint8_t value, bool useBuffer) {
  if (useBuffer) {
    drawIntoBuffer(value);
  }
  else {
    drawPixel(value);
  }
}

void ssd1327::drawIntoBuffer(uint8_t value) {
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

void ssd1327::drawPixel(uint8_t value) {
    dataStart();
    sendByte(value);
    stopTransmission();
    
    drawBufferCursor = 0;
    combineBuffer = false;
}

void ssd1327::drawAllBuffer() {
  if (drawBufferCursor > 0) {
    sendBytes(drawBuffer, drawBufferCursor);

    drawBufferCursor = 0;
    combineBuffer = false;
  }
}

void ssd1327::drawCompressedBitmap(const uint8_t arr[], const uint8_t sequences[], uint16_t offset, uint8_t arrayLength, uint8_t x, uint8_t y, uint8_t sizeX, uint8_t sizeY) {
  setDrawRange(x, y, sizeX, sizeY);

  for (uint16_t j = offset; j < offset + arrayLength; j++) {
    uint16_t sequenceId = pgm_read_byte_near(arr + j);
    uint8_t repeatCount = pgm_read_byte_near(sequences + sequenceId);
    uint8_t value = pgm_read_byte_near(sequences + sequenceId + 1);

    for (uint16_t i = 0; i < repeatCount; i++) {
      draw(value, true);
    }
  }

  drawAllBuffer();
}

void ssd1327::drawBitmapUsingPalette(const bool arr[], const uint8_t palette[], uint16_t offset, const uint8_t repeats[], uint8_t arrayLength, uint8_t x, uint8_t y, uint8_t sizeX, uint8_t sizeY) {
  setDrawRange(x, y, sizeX, sizeY);

  for (uint16_t j = offset; j <= offset + arrayLength; j++) {
    uint8_t color = pgm_read_byte_near(palette + pgm_read_byte_near(arr + j));
    uint8_t repeatCount = pgm_read_byte_near(repeats + j);

    for (uint16_t i = 0; i < repeatCount; i++) {
      draw(color, true);
    }
  }

  drawAllBuffer();
}

void ssd1327::drawBitmap(const uint8_t arr[], uint8_t x, uint8_t y, uint8_t sizeX, uint8_t sizeY) {
  uint16_t arrayLength = sizeX * sizeY;

  setDrawRange(x, y, sizeX, sizeY);

  for (uint16_t j = 0; j < arrayLength; j++) {
    draw(pgm_read_byte_near(arr + j), true);
  }

  drawAllBuffer();
}

void ssd1327::clear(uint8_t color) {
  setDrawRange(0, 0, width, height);
  
	for (uint8_t x = 0; x < width; x++) {
    for (uint8_t y = 0; y < height; y++) {
      draw(color, true);
    }
	}

 drawAllBuffer();
}

void ssd1327::clear(uint8_t color, uint8_t x, uint8_t y, uint8_t sizeX, uint8_t sizeY) {
  uint16_t arrayLength = sizeX * sizeY;

  setDrawRange(x, y, sizeX, sizeY);

  for (uint16_t j = 0; j < arrayLength; j++) {
    draw(color, true);
  }

  drawAllBuffer();
}

void ssd1327::clearBorder(uint8_t color, uint8_t x, uint8_t y, uint8_t sizeX, uint8_t sizeY) {
  setDrawRange(x, y, sizeX, 1);

  // clear top line
  for (uint8_t j = 0; j < sizeX; j++) {
    draw(color, true);
  }

  drawAllBuffer();

  setDrawRange(x, y, 0, sizeY);

  // clear left line
  for (uint8_t j = 0; j < sizeY; j++) {
    draw(color, true);
    draw(color, true);
  }

  drawAllBuffer();

  setDrawRange(x + sizeX / 2, y, 0, sizeY);
  
  // clear right line
  for (uint8_t j = 0; j < sizeY; j++) {
    draw(color, true);
    draw(color, true);
  }

  drawAllBuffer();
  
  setDrawRange(x, y + sizeY - 1, sizeX, 1);
  
  // clear bottom line
  for (uint8_t j = 0; j < sizeX; j++) {
    draw(color, true);
  }

  drawAllBuffer();
}

void ssd1327::on() {
  sendCommand(SSD1327_DISPLAY_ON_COMMAND);
}

void ssd1327::off() {
  sendCommand(SSD1327_DISPLAY_OFF_COMMAND);
}

void ssd1327::remap(uint8_t value) {
  sendCommand(SSD1327_REMAP_COMMAND, value);
}

void ssd1327::remap(uint8_t value1, uint8_t value2) {
  sendCommand(SSD1327_REMAP_COMMAND, value1, value2);
}

void ssd1327::multiplex(uint8_t value) {
  sendCommand(SSD1327_SET_MULTIPLEX_COMMAND, value);
}