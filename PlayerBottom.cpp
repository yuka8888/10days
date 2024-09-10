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

void PlayerBottom::PlayerBottomPhaseUpdate()
{
	Move();
}

void PlayerBottom::PlayerTopPhaseUpdate()
{
	velocity_ = { 0.0f, 0.0f };
	isPushTwoBlocks_ = false;

}

void PlayerBottom::Draw(Camera camera)
{
	//スクリーン座標に変換
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translation_);
	wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, camera.worldMatrix, camera.vertex, camera.viewPortPosition, camera.viewPortSize);

	screenPosition_ = Transform(initialPosition_, wvpVpMatrix_);

	Novice::DrawBox(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f), (int)kWidth_, (int)kHeight_, 0.0f, BLUE, kFillModeWireFrame);

	switch (direction)
	{
	case kRight:

		Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
			int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
			(int)kWidth_ * (animationTimer / 7), 0, (int)kWidth_, (int)kHeight_, walkRight,
			WHITE);

		break;
	case kRightStand:
		Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
			int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
			(int)kWidth_ * (animationTimer / 30), 0, (int)kWidth_, (int)kHeight_, standingRight,
			WHITE);
		break;
	case kLeft:

		Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
			int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
			(int)kWidth_ * (animationTimer / 7), 0, (int)kWidth_, (int)kHeight_, walkLeft,
			WHITE);

		break;
	case kLeftStand:
		Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
			int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
			(int)kWidth_ * (animationTimer / 30), 0, (int)kWidth_, (int)kHeight_, standingLeft,
			WHITE);
		break;
	case kPushBlock:
		Novice::DrawQuad(int(screenPosition_.x - pushWidth / 2.0f) - 7, int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x + pushWidth / 2.0f) - 7, int(screenPosition_.y - kHeight_ / 2.0f),
			int(screenPosition_.x - pushWidth / 2.0f) - 7, int(screenPosition_.y + kHeight_ / 2.0f),
			int(screenPosition_.x + pushWidth / 2.0f) - 7, int(screenPosition_.y + kHeight_ / 2.0f),
			(int)pushWidth * (animationTimer / 7), 0, (int)pushWidth, (int)kHeight_, pushBlock,
			WHITE);
		break;

	case kRightJump:
		break;
	case kLeftJump:
		break;
	default:
		break;
	}

#ifdef _DEBUG
	ImGui::DragFloat2("PlayerBoy.Translation", &translation_.x, 0.01f);
	ImGui::DragFloat2("PlayerBoy.Velocity", &velocity_.x, 0.01f);
#endif // _DEBUG

}

void PlayerBottom::Move()
{
	isPushTwoBlocks_ = false;
	isPushBlock_ = false;

	animationTimer++;
	AnimationTimerChange();
	velocity_ = { 0.0f, 0.0f };
	//directionを待機に切り替え
	if (direction == Direction::kLeft) {
		direction = Direction::kLeftStand;
	}
	else if (direction == Direction::kRight) {
		direction = Direction::kRightStand;
	}
	//移動処理
	if (Novice::CheckHitKey(DIK_A) || Novice::CheckHitKey(DIK_LEFTARROW)) {
		direction = Direction::kLeft;
		velocity_.x = -2.0f;
	}
	else if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		direction = Direction::kRight;
		velocity_.x = 2.0f;
	}
	////止まったらdirectionを待機に切り替え
	if (velocity_.x == 0) {
		if (direction == Direction::kRight || direction == Direction::kPushBlock) {

			direction = Direction::kRightStand;
		}
		else if (direction == Direction::kLeft) {
			direction = Direction::kLeftStand;
		}
	}

	////ブロックが押されていたらdirectionをpushに切り替え
	//if (isPushBlock_) {
	//	direction = Direction::kPushBlock;
	//}
	//if (isPushBlock_ == false) {
	//	direction = Direction::kRightStand;
	//}
	// 
	//進んだ先のaabbの更新
	aabb_.max = { translation_.x + velocity_.x + kWidth_ / 2, translation_.y + velocity_.y + kHeight_ / 2 };
	aabb_.min = { translation_.x + velocity_.x - kWidth_ / 2, translation_.y + velocity_.y - kHeight_ / 2 };

}


void PlayerBottom::MoveResult()
{
	if (!isPushTwoBlocks_) {
		translation_ = translation_ + velocity_;
	}
	else {
		translation_.y = translation_.y + velocity_.y;
	}
}

void PlayerBottom::PushTwoBlocks(Block block)
{
	if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		isPushTwoBlocks_ = true;
		translation_.x = block.initialPosition.x + block.velocity.x - 24.0f - kWidth_ / 2.0f;
	}
}

AABB PlayerBottom::GetAABB()
{
	return aabb_;
}

void PlayerBottom::OnCollision()
{
	isPushBlock_ = true;

	//右が押されていたら
	if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		//押してる
		direction = Direction::kPushBlock;
	}

	//移動量を減らす
	velocity_.x = velocity_.x * 0.30f;

#ifdef _DEBUG
	ImGui::InputInt("PlayerBoy.isPushBlock_", &isPushBlock_);
#endif // _DEBUG

}

Vector2 PlayerBottom::GetVelocity()
{
	return velocity_;
}

Vector2 PlayerBottom::GetTranslation()
{
	return translation_;
}

Direction PlayerBottom::GetDirection()
{
	return direction;
}

void PlayerBottom::isPushTwoBlocks(bool isPushTwoBlocks)
{
	isPushTwoBlocks_ = isPushTwoBlocks;
}

Vector2 PlayerBottom::GetSize()
{
	return { kWidth_, kHeight_ };
}

void PlayerBottom::SetTranslation(Vector2 translation)
{
	translation_ = translation;
}

void PlayerBottom::AnimationTimerChange()
{

	switch (direction)
	{
	case kRight:
		animationTimerReset = 56;
		break;
	case kRightStand:
		animationTimerReset = 60;
		break;
	case kLeft:
		animationTimerReset = 56;
		break;
	case kLeftStand:
		animationTimerReset = 60;
		break;
	case kPushBlock:
		animationTimerReset = 63;
		break;
	case kRightJump:
		break;
	case kLeftJump:
		break;

	default:
		break;
	}

	if (animationTimer >= animationTimerReset) {
		animationTimer = 0;
	}
}
