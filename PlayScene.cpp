#include "PlayScene.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
	delete playerTop_;
	delete playerBottom_;
	delete cameraManager_;
}

void PlayScene::Initialize()
{
	//プレイヤーの初期化
	playerTop_ = new PlayerTop;
	playerTop_->Initialize();
	playerBottom_ = new PlayerBottom;
	playerBottom_->Initialize();

	//カメラの初期化
	cameraManager_ = new CameraManager;
	cameraManager_->Initialize();
}

void PlayScene::Update()
{
	//プレイヤーの更新
	playerTop_->Update();
	playerBottom_->Update();

	if (Novice::CheckHitKey(DIK_SPACE)) {
		sceneNo = kClear;
	}

	

}

void PlayScene::Draw()
{

	//プレイヤーの描画
	playerTop_->Draw(cameraManager_->GetCamera());
	playerBottom_->Draw(cameraManager_->GetCamera());
}
