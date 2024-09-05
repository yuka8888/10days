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

void PlayerTop::Update()
{
	Move();
}

void PlayerTop::Draw(Camera camera)
{
	//スクリーン座標に変換
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translation_);
	wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, camera.worldMatrix, camera.vertex, camera.viewPortPosition, camera.viewPortSize);

	screenPosition_ = Transform(initialPosition_, wvpVpMatrix_);

	Novice::DrawBox((int)screenPosition_.x, (int)screenPosition_.y, 48, 60, 0.0f, RED, kFillModeSolid);
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
	if (isGround && (Novice::CheckHitKey(DIK_W) || Novice::CheckHitKey(DIK_UPARROW))) {
		isGround = false;
		isJump = true;

		velocity_.y = 10.0f;
	}

	//ジャンプ中
	if (isJump) {
		velocity_ = velocity_ + kAcceleration_;

		//地面についたら接地状態に切り替え
		if (velocity_.y < 0 && translation_.y <= 0) {
			isGround = true;
			translation_.y = 0;
			velocity_.y = 0;
		}
	}

	translation_ = translation_ + velocity_;
}
