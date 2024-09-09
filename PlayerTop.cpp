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

}

void PlayerTop::MoveResult()
{
	//前の座標を記憶
	preTranslation_ = translation_;

	translation_ = translation_ + velocity_;

}

void PlayerTop::Draw(Camera camera)
{
	//スクリーン座標に変換
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translation_);
	wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, camera.worldMatrix, camera.vertex, camera.viewPortPosition, camera.viewPortSize);


	screenPosition_ = Transform(initialPosition_, wvpVpMatrix_);

	Novice::DrawBox(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f), (int)kWidth_, (int)kHeight_, 0.0f, RED, kFillModeSolid);

	ImGui::DragFloat2("PlayerGirl.Translation", &translation_.x, 0.01f);
}

void PlayerTop::Move()
{
	velocity_.x = 0.0f;

	isLanding_ = false;
	isCollideCeiling_ = false;
	isContactWall_ = false;

	//当たり判定下
	MapCollisionBottom();

	//ジャンプ中
	//地面についていて上押されたらジャンプ開始
	if (isGround_ && (Novice::CheckHitKey(DIK_W) || Novice::CheckHitKey(DIK_UPARROW))) {
		isGround_ = false;
		isJump = true;

		velocity_.y = 10.0f;
	}

	if (isJump) {
		velocity_ = velocity_ + kAcceleration_;
		//天井との当たり判定
		MapCollisionTop();

		//地面についたら接地状態に切り替え
		if (velocity_.y < 0 && translation_.y <= kGround_) {
			isGround_ = true;
			translation_.y = kGround_;
			velocity_.y = 0;
		}
	}

	//移動処理
	if (Novice::CheckHitKey(DIK_A) || Novice::CheckHitKey(DIK_LEFTARROW)) {
		direction = Direction::kLeft;
		velocity_.x = -2.0f;

		//当たり判定
		MapCollisionLeft();
	}
	else if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {		
		direction = Direction::kRight;
		velocity_.x = 2.0f;

		//当たり判定
		MapCollisionRight();
	}

	if (isGround_) {
		velocity_.y = 0.0f;
		translation_.y = kGround_;
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

void PlayerTop::MapCollisionTop()
{
	// 移動後の4つの角の座標
	std::array<Vector2, Corner::kNumCorner> positionNew;
	
	// 真上の当たり判定を行う
	//移動後の座標
	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(translation_ + velocity_, static_cast<Corner>(i));
	}
	
	//左上
	IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftTop] + Vector2{ 1.0f, 0.0f });

	if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isCollideCeiling_ = true;
	}

	//右上
	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightTop] + Vector2{-1.0f, 0.0f});

	if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isCollideCeiling_ = true;
	}

}

void PlayerTop::MapCollisionBottom()
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

	if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isLanding_ = true;
	}

	//右下
	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector2{ -1.0f, 0.0f });

	if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isLanding_ = true;
	}

}

void PlayerTop::MapCollisionRight()
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

		if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
	}

	//右真ん中
	IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(translation_ + velocity_ + Vector2{kWidth_ / 2.0f, 0.0f});

	if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
		velocity_.x = 0.0f;
	}

	//ブロックが下にあったら当たり判定とらない
	if (!isLanding_) {
		//右下
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);

		if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
	}

}

void PlayerTop::MapCollisionLeft()
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
	}

	//左真ん中
	IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(translation_ + velocity_ - Vector2{kWidth_ / 2.0f, 0.0f});

	if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
		velocity_.x = 0.0f;
	}

	//ブロックが下にあったら当たり判定とらない
	if (!isLanding_) {
		//左下
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);

		if (MapChipType::kBlock == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
	}

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
