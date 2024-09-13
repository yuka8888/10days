#include "color.h"

// 色の変換
unsigned int GetColor(unsigned int red, unsigned int blue, unsigned int green, unsigned int alpha) {

	int color = 0;

	red <<= 24;
	green <<= 16;
	blue <<= 8;

	color = red + green + blue + alpha;

	return color;
}

//static unsigned int GetColor(Color color) {
//
//	int colorBit = 0;
//
//	color.red <<= 24;
//	color.blue <<= 16;
//	color.green <<= 8;
//
//	colorBit = color.red + color.green + color.blue + color.alpha;
//
//	return colorBit;
//}