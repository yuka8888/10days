#include "PlayerTop.h"

PlayerTop::PlayerTop()
{
}

PlayerTop::~PlayerTop()
{
}

void PlayerTop::Initialize()
{
}

void PlayerTop::PlayerBottomMoveUpdate()
{
	Jump();

	translation_ = translation_ + velocity_;

	//aabbの更新
	aabb_.max = { translation_.x + kWidth_ / 2, translation_.y + kHeight_ / 2 };
	aabb_.min = { translation_.x - kWidth_ / 2, translation_.y - kHeight_ / 2 };

}

void PlayerTop::PlayerTopMoveUpdate()
{
	Move();
}

void PlayerTop::Draw(Camera camera)
{
	//スクリーン座標に変換
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translation_);
	wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, camera.worldMatrix, camera.vertex, camera.viewPortPosition, camera.viewPortSize);

	screenPosition_ = Transform(initialPosition_, wvpVpMatrix_);

	Novice::DrawBox(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f), (int)kWidth_, (int)kHeight_, 0.0f, RED, kFillModeSolid);
}

void PlayerTop::Jump()
{
	//ジャンプ中
	if (isJump_) {
		velocity_ = velocity_ + kAcceleration_;

		//地面についたら接地状態に切り替え
		if (velocity_.y < 0 && translation_.y <= kGround_) {
			isGround_ = true;
			translation_.y = kGround_;
			velocity_.y = 0;
		}
	}

}

void PlayerTop::Move()
{
	velocity_.x = 0.0f;

	//移動処理
	if (Novice::CheckHitKey(DIK_A) || Novice::CheckHitKey(DIK_LEFTARROW)) {
		velocity_.x = -2.0f;
	}
	else if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		velocity_.x = 2.0f;
	}


	//地面についていて上押されたらジャンプ開始
	if (isGround_ && (Novice::CheckHitKey(DIK_W) || Novice::CheckHitKey(DIK_UPARROW))) {
		isGround_ = false;
		isJump_ = true;

		velocity_.y = 10.0f;
	}

	translation_ = translation_ + velocity_;

	//aabbの更新
	aabb_.max = { translation_.x + kWidth_ / 2, translation_.y + kHeight_ / 2 };
	aabb_.min = { translation_.x - kWidth_ / 2, translation_.y - kHeight_ / 2 };
}

AABB PlayerTop::GetAABB()
{
	return aabb_;
}

void PlayerTop::OnCollision()
{
}
