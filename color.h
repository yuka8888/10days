#pragma once

// 色の変換
struct Color final {
	int red;
	int blue;
	int green;
	int alpha;
};
unsigned int GetColor(unsigned int red, unsigned int blue, unsigned int green, unsigned int alpha);
//static unsigned int GetColor(Color color);

