#include "MapChipManager.h"
#include <fstream>
#include <map>
#include <sstream>

namespace {
	std::map<std::string, MapChipType> mapChipTable = {
		{"0", MapChipType::kBlank},
		{"1", MapChipType::kGround_},
		{"2", MapChipType::kBlock},
	};
}

void MapChipManager::ResetMapChipData()
{
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {

		mapChipDataLine.resize(kNumBlockHorizontal);
	}

}
void MapChipManager::LoadMapChipCsv(const std::string& filePath) {
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる
	file.close();

	std::string line;

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[(i - kNumBlockVirtical + 1) * -1][j] = mapChipTable[word];

				//ブロックの個数を取得
				if (mapChipTable[word] == MapChipType::kBlock) {
					kBlockNum++;

				}
			}
		}
	}
}



uint32_t MapChipManager::GetNumBlockVirtical()
{
	return kNumBlockVirtical;
}

uint32_t MapChipManager::GetNumBlockHorizontal()
{
	return kNumBlockHorizontal;
}

MapChipData MapChipManager::GetMapChipDate()
{
	return mapChipData_;
}

uint32_t MapChipManager::GetBlockNum()
{
	return kBlockNum;
}
