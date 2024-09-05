#include "PlayerBottom.h"
PlayerBottom::PlayerBottom()
{
}

PlayerBottom::~PlayerBottom()
{
}

void PlayerBottom::Initialize()
{
}

void PlayerBottom::Update()
{
	Move();
}

void PlayerBottom::Draw(Camera camera)
{
	//スクリーン座標に変換
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translation_);
	wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, camera.worldMatrix, camera.vertex, camera.viewPortPosition, camera.viewPortSize);

	screenPosition_ = Transform(initialPosition_, wvpVpMatrix_);

	Novice::DrawBox((int)screenPosition_.x, (int)screenPosition_.y, 48, 60, 0.0f, BLUE, kFillModeSolid);
}

void PlayerBottom::Move()
{
	velocity_ = { 0.0f, 0.0f };

	//移動処理
	if (Novice::CheckHitKey(DIK_A) || Novice::CheckHitKey(DIK_LEFTARROW)) {
		velocity_.x = -2.0f;
	}
	else if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		velocity_.x = 2.0f;
	}

	translation_ = translation_ + velocity_;

}
