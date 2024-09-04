#include "SceneManager.h"

SceneManager::SceneManager()
{
	//各シーンの配列
	sceneArr_[kTitle] = std::make_unique<TitleScene>();
	sceneArr_[kPlay] = std::make_unique<PlayScene>();
	sceneArr_[kClear] = std::make_unique<ClearScene>();

	//初期シーンの設定
	currentSceneNo_ = kTitle;
}


SceneManager::~SceneManager()
{
}

int SceneManager::Run()
{
	//シーンのチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

	//シーン変更チェック
	if (prevSceneNo_ != currentSceneNo_) {
		sceneArr_[currentSceneNo_]->Initialize();
	}

	//更新処理
	sceneArr_[currentSceneNo_]->Update(); //シーンごとの更新処理

	//描画処理
	sceneArr_[currentSceneNo_]->Draw();

	return 0;
}
