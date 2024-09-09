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

	Novice::DrawBox(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f), (int)kWidth_, (int)kHeight_, 0.0f, BLUE, kFillModeSolid);

#ifdef _DEBUG
	ImGui::DragFloat2("PlayerBoy.Translation", &translation_.x, 0.01f);
	ImGui::DragFloat2("PlayerBoy.Velocity", &velocity_.x, 0.01f);
#endif // _DEBUG

}

void PlayerBottom::Move()
{
	isPushTwoBlocks_ = false;

	velocity_ = { 0.0f, 0.0f };

	//移動処理
	if (Novice::CheckHitKey(DIK_A) || Novice::CheckHitKey(DIK_LEFTARROW)) {
		direction = Direction::kLeft;
		velocity_.x = -2.0f;
	}
	else if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		direction = Direction::kRight;
		velocity_.x = 2.0f;
	}
	 
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
