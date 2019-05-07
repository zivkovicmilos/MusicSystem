#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "Formatter.h";
#include <tuple>
#include <regex>
#include <cstdint>

class BMPFormatter : Formatter {
	map<string, tuple<int, int, int>> colorMap;
	vector<uint8_t> red;
	vector<uint8_t> green;
	vector<uint8_t> blue;

	struct HDR {
		uint16_t type = 0x4d42; // 'B' 'M'
		uint32_t size; // Total file size
		uint32_t zeroes1 = 0; // 00 00
		uint32_t offset = 54; // Offset for pixel data (54B)
		uint32_t DIB = 40;   // DIB Header size (40B)
		int32_t  width; // image width
		int32_t  height; // image height
		uint16_t one = 1; // 1 (fixed value)
		uint16_t bpp = 24; // 24
		uint32_t zeroes3 = 0; // 00 00 00 00
		uint32_t totalSize; // total image size in bytes
		int32_t  fixed1 = 2835; // 2835
		int32_t  fixed2 = 2835; // 2835
		uint32_t fixed3 = 0; //00 00 00 00
		uint32_t fixed4 = 0; // 00 00 00 00
	};

	struct IMG {
		unsigned char* imgData;
		HDR header;
		IMG(uint32_t size, uint32_t width, uint32_t height, uint32_t totalSize) {
			this->header.size = size;
			this->header.width = width;
			this->header.height = height;
			this->header.totalSize = totalSize;
		}
	};

	IMG* actualImage;
	int width;
public:
	BMPFormatter(Composition*, int);

	void readColorMap();
	int getValue(string, int);
	void pushColors(int, int, int);
	void addColors(int, string);
	uint8_t getColorMix(vector<uint8_t>&);
	void generateBoilerplate(ofstream&, uint32_t, uint32_t, uint32_t);
	string numToHex(int num);

	void format() override;
};