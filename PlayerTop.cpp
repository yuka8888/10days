#include "PlayerTop.h"
#include "MapChipManager.h"

PlayerTop::PlayerTop()
{
}

PlayerTop::~PlayerTop()
{
}

void PlayerTop::Initialize()
{
}

void PlayerTop::PlayerBottomPhaseUpdate()
{
	velocity_.x = 0.0f;

	//ジャンプ中
	if (isJump) {
		velocity_ = velocity_ + kAcceleration_;

		//地面についたら接地状態に切り替え
		if (velocity_.y < 0 && translation_.y <= kGround_) {
			isGround_ = true;
			translation_.y = kGround_;
			velocity_.y = 0;
		}
	}


	//aabbの更新
	aabb_.max = { translation_.x + kWidth_ / 2, translation_.y + kHeight_ / 2 };
	aabb_.min = { translation_.x - kWidth_ / 2, translation_.y - kHeight_ / 2 };

}

void PlayerTop::PlayerTopPhaseUpdate()
{
	Move();
	MapCollisionBottom();
}

void PlayerTop::MoveResult()
{
	translation_ = translation_ + velocity_;

}

void PlayerTop::Draw(Camera camera)
{
	//スクリーン座標に変換
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translation_);
	wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, camera.worldMatrix, camera.vertex, camera.viewPortPosition, camera.viewPortSize);

	screenPosition_ = Transform(initialPosition_, wvpVpMatrix_);

	Novice::DrawBox(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f), (int)kWidth_, (int)kHeight_, 0.0f, RED, kFillModeSolid);
}

void PlayerTop::Move()
{
	velocity_.x = 0.0f;

	//移動処理
	if (Novice::CheckHitKey(DIK_A) || Novice::CheckHitKey(DIK_LEFTARROW)) {
		direction = Direction::kLeft;
		velocity_.x = -2.0f;
	}
	else if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		direction = Direction::kRight;
		velocity_.x = 2.0f;
	}

	//地面についていて上押されたらジャンプ開始
	if (isGround_ && (Novice::CheckHitKey(DIK_W) || Novice::CheckHitKey(DIK_UPARROW))) {
		isGround_ = false;
		isJump = true;

		velocity_.y = 10.0f;
	}

	//ジャンプ中
	if (isJump) {
		velocity_ = velocity_ + kAcceleration_;

		//地面についたら接地状態に切り替え
		if (velocity_.y < 0 && translation_.y <= kGround_) {
			isGround_ = true;
			translation_.y = kGround_;
			velocity_.y = 0;
		}
	}

	if (isGround_) {
		velocity_.y = 0.0f;
	}

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

void PlayerTop::SetMapChipField(MapChipManager* mapChipManager)
{
	mapChipManager_ = mapChipManager;
}

Vector2 PlayerTop::CornerPosition(const Vector2& center, Corner corner) {
	Vector2 offsetTable[kNumCorner] = {
		{+kWidth_ / 2.0f, -kHeight_ / 2.0f},
		{-kWidth_ / 2.0f, -kHeight_ / 2.0f},
		{+kWidth_ / 2.0f, +kHeight_ / 2.0f},
		{-kWidth_ / 2.0f, +kHeight_ / 2.0f}
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}


void PlayerTop::MapCollisionBottom() {
	// 下降あり？
	if (velocity_.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector2, Corner::kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(translation_ + velocity_, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり判定を行う
	bool hit = false;

	// 左下点の判定
	IndexSet indexSet;

	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kGround_) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kGround_) {
		hit = true;
	}


	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
		// めり込み先ブロックの矩形範囲
		Rect rect = mapChipManager_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		velocity_.y = std::min(0.0f, (rect.top - translation_.y) + kHeight_ / 2);
		// 地面に当たったことを記録する
		isGround_ = true;
	}
}

