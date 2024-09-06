#include "ClearScene.h"

ClearScene::ClearScene()
{
}

void ClearScene::Initialize()
{
}

void ClearScene::PlayerBottomMoveUpdate()
{
	if (Novice::CheckHitKey(DIK_RETURN)) {
		sceneNo = kTitle;
	}

}

void ClearScene::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, RED, kFillModeSolid);

}
