class ssd1327 {
	private:
		static const uint8_t column = 0x3F;
		static const uint8_t row = 0x7F;


	public:
		ssd1327();
		/*ssd1327(void (*wireBeginFunc)(void),
				bool (*wireBeginTransmissionFunc)(void), bool (*wireWriteFunc)(uint8_t byte),
				uint8_t (*wireEndTransmissionFunc)(void));*/
				
		void off(void);
		void on(void);
		
		void drawBitmap(const uint8_t arr[], uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y);
    void drawCompressedBitmap(const uint8_t arr[], const uint8_t sequences[], uint16_t offset, uint8_t arrayLength, uint8_t sizeX, uint8_t sizeY, uint8_t x, uint8_t y);
		void drawPixels(uint16_t count, uint8_t color);
		void remap(uint8_t value);
    void remap(uint8_t value1, uint8_t value2);
    void multiplex(uint8_t value);
    void clear(uint8_t color, uint8_t xSize, uint8_t ySize, uint8_t x, uint8_t y);
    void clearBorder(uint8_t color, uint8_t xSize, uint8_t ySize, uint8_t x, uint8_t y);
		void clear(uint8_t color);
};
