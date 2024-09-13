#include "SceneManager.h"

SceneManager::SceneManager()
{
	//各シーンの配列
	sceneArr_[kTitle] = new TitleScene;
	sceneArr_[kPlay] = new PlayScene;
	sceneArr_[kClear] = new ClearScene;

	//初期シーンの設定
	currentSceneNo_ = kTitle;
	sceneArr_[currentSceneNo_]->Initialize();
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
		delete sceneArr_[currentSceneNo_];
		if (currentSceneNo_ == kTitle) {
			sceneArr_[currentSceneNo_] = new TitleScene;
		}
		else if (currentSceneNo_ == kPlay) {
			sceneArr_[currentSceneNo_] = new PlayScene;
		}
		else if (currentSceneNo_ == kClear) {
			sceneArr_[currentSceneNo_] = new ClearScene;
		}
		sceneArr_[currentSceneNo_]->Initialize();
	}

	//更新処理
	sceneArr_[currentSceneNo_]->Update(); //シーンごとの更新処理

	if (currentSceneNo_ == kPlay) {
		if (sceneArr_[currentSceneNo_]->IsStageClear()) {
			sceneArr_[currentSceneNo_]->Initialize();
		}
	}

	//描画処理
	sceneArr_[currentSceneNo_]->Draw();

	return 0;
}
