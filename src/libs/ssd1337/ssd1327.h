#define DRAW_BUFFER_SIZE 8

#include "vector2.h"

class ssd1327 {
	private:
		static const uint8_t column = 0x3F;
		static const uint8_t row = 0x7F;

    static const uint8_t width = 128;
    static const uint8_t height = 96;

    uint8_t drawBuffer[DRAW_BUFFER_SIZE];
    uint8_t drawBufferCursor;
    bool combineBuffer = false;

    void draw(uint8_t value, bool useBuffer, bool skip);
    void draw(uint8_t value, bool useBuffer);
    void drawIntoBuffer(uint8_t value);
    void drawPixel(uint8_t value);
    void drawAllBuffer();

	public:
		ssd1327();
    
    void startTransmission();
    void sendByte(uint8_t byte);
    void sendBytes(uint8_t *byte, uint8_t length);
    void sendBytesAsOne(uint8_t *byte, uint8_t length);
    void stopTransmission(void);
    void commandStart(void);
    void dataStart(void);
    void sendCommand(uint8_t command1);
    void sendCommand(uint8_t command1, uint8_t command2);
    void sendCommand(uint8_t command1, uint8_t command2, uint8_t command3);

    void setCursor(uint8_t x, uint8_t y);
    void setDrawRange(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

    void off(void);
    void on(void);
		
		void drawBitmap(const uint8_t arr[], uint8_t x, uint8_t y, uint8_t sizeX, uint8_t sizeY);
    void drawCompressedBitmap(const uint8_t arr[], const uint8_t sequences[], uint16_t offset, uint8_t arrayLength, uint8_t x, uint8_t y, uint8_t sizeX, uint8_t sizeY);
    void drawBitmapUsingPalette(const bool arr[], const uint8_t palette[], uint16_t offset, const uint8_t repeats[], uint8_t arrayLength, uint8_t x, uint8_t y, uint8_t sizeX, uint8_t sizeY);
		void drawPixels(uint16_t count, uint8_t color);
   
		void remap(uint8_t value);
    void remap(uint8_t value1, uint8_t value2);
    void multiplex(uint8_t value);
    
		void clear(uint8_t color);
    void clear(uint8_t color, uint8_t x, uint8_t y, uint8_t xSize, uint8_t ySize);
    void clearBorder(uint8_t color, uint8_t x, uint8_t y, uint8_t xSize, uint8_t ySize);
};