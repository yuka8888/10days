#include "TitleScene.h"

TitleScene::TitleScene()
{
}

void TitleScene::Initialize()
{
}

void TitleScene::PlayerBottomMoveUpdate()
{
	if (Novice::CheckHitKey(DIK_Q)) {
		sceneNo = kPlay;
	}
}

void TitleScene::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, BLUE, kFillModeSolid);

}
