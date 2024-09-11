#include "PlayerBottom.h"
#include "MapChipManager.h"

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
	if (!isGoal) {
		Move();
	}
	else {
		velocity_ = {};
	}

	//鍵を持った状態じゃないとゴールにしない
	if (!haveKey_) {
		isGoal = false;
	}

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

	MapCollisionBottom();

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
		MapCollisionLeft();
	}
	else if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		direction = Direction::kRight;
		velocity_.x = 2.0f;
		MapCollisionRight();
	}
	
	//止まったらdirectionを待機に切り替え
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

void PlayerBottom::SetMapChipField(MapChipManager* mapChipManager)
{
	mapChipManager_ = mapChipManager;
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

void PlayerBottom::HaveKey(bool haveKey)
{
	haveKey_ = haveKey;
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

Vector2 PlayerBottom::CornerPosition(const Vector2& center, Corner corner)
{
	Vector2 offsetTable[kNumCorner] = {
		{+kWidth_ / 2.0f, -kHeight_ / 2.0f},
		{-kWidth_ / 2.0f, -kHeight_ / 2.0f},
		{+kWidth_ / 2.0f, +kHeight_ / 2.0f},
		{-kWidth_ / 2.0f, +kHeight_ / 2.0f}
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];


}


void PlayerBottom::MapCollisionBottom()
{
	// 移動後の4つの角の座標
	std::array<Vector2, Corner::kNumCorner> positionNew;

	// 真下の当たり判定を行う
	//移動後の座標
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(translation_ + velocity_, static_cast<Corner>(i));
	}

	//左下
	IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector2{ 1.0f, 0.0f });

	if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		haveKey_ = true;
	}

	if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isGoal = true;
	}

	//右下
	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector2{ -1.0f, 0.0f });

	if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		haveKey_ = true;
	}

	if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isGoal = true;
	}

}

void PlayerBottom::MapCollisionRight()
{
	// 移動後の4つの角の座標
	std::array<Vector2, Corner::kNumCorner> positionNew;

	// 右の当たり判定を行う
	//移動後の座標
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(translation_ + velocity_, static_cast<Corner>(i));
	}

	//天井に当たっていたら上は当たり判定とらない
	if (!isCollideCeiling_) {
		//右上
		IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);

		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal = true;
		}

	}

	//右真ん中
	IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(translation_ + velocity_ + Vector2{ kWidth_ / 2.0f, 0.0f });

	if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		haveKey_ = true;
	}

	//ブロックが下にあったら当たり判定とらない
	if (!isGround_) {
		//右下
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);

		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal = true;
		}

	}

}

void PlayerBottom::MapCollisionLeft()
{
	// 移動後の4つの角の座標
	std::array<Vector2, Corner::kNumCorner> positionNew;

	// 左の当たり判定を行う
	//移動後の座標
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(translation_ + velocity_, static_cast<Corner>(i));
	}

	//天井に当たっていたら上は当たり判定とらない
	if (!isCollideCeiling_) {
		//左上
		IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);

		if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}

		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal = true;
		}

	}

	//左真ん中
	IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(translation_ + velocity_ - Vector2{ kWidth_ / 2.0f, 0.0f });

	if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
		velocity_.x = 0.0f;
	}

	//ブロックが下にあったら当たり判定とらない
	if (!isGround_) {
		//左下
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);

		if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}

		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal = true;
		}

	}

}
