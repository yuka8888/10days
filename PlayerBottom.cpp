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

	Novice::DrawBox(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f), (int)kWidth_, (int)kHeight_, 0.0f, BLUE, kFillModeSolid);
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

	//aabbの更新
	aabb_.max = { translation_.x + kWidth_ / 2, translation_.y + kHeight_ / 2 };
	aabb_.min = { translation_.x - kWidth_ / 2, translation_.y - kHeight_ / 2 };

}

void PlayerBottom::PushBlock()
{

}

AABB PlayerBottom::GetAABB()
{
	return aabb_;
}

void PlayerBottom::OnCollision()
{
	isPushBlock_ = true;

	//移動量を減らす
	translation_.x = translation_.x - velocity_.x * 0.7f;
}

Vector2 PlayerBottom::GetVelocity()
{
	return velocity_;
}
