#include "PlayScene.h"

PlayScene::PlayScene()
{
}

void PlayScene::Initialize()
{
}

void PlayScene::Update()
{
	if (Novice::CheckHitKey(DIK_SPACE)) {
		sceneNo = kClear;
	}

}

void PlayScene::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, WHITE, kFillModeSolid);
}
