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

void PlayerTop::PlayerStopPhaseUpdate()
{
	velocity_.x = 0.0f;
	animationTimer++;

	//状態によってタイマーのリセット値を変える
	AnimationTimerChange();

	//当たり判定下
	MapCollisionBottom();

	//ジャンプ中
	if (isJump) {
		velocity_ = velocity_ + kAcceleration_;

		//最高速度を制限
		if (kMaxFallSpeed >= velocity_.y) {
			velocity_.y = kMaxFallSpeed;
		}

		//地面についたら接地状態に切り替え
		if (velocity_.y < 0 && translation_.y <= nowGround_) {
			isGround_ = true;
			translation_.y = nowGround_;
			velocity_.y = 0;
		}
	}

	////directionを待機に切り替え
	if (velocity_.x == 0) {
		if (direction == Direction::kRight) {

			direction = Direction::kRightStand;
		}
		else if (direction == Direction::kLeft) {
			direction = Direction::kLeftStand;
		}
	}

	//aabbの更新
	aabb_.max = { translation_.x + kWidth_ / 2, translation_.y + kHeight_ / 2 };
	aabb_.min = { translation_.x - kWidth_ / 2, translation_.y - kHeight_ / 2 };

}

void PlayerTop::PlayerMovePhaseUpdate()
{
	animationTimer++;
	//状態によってタイマーのリセット値を変える
	AnimationTimerChange();

	if (!isGoal_) {
		Move();
	}
	else {
		velocity_.x = 0.0f;

		if (direction == Direction::kRight) {

			direction = Direction::kRightStand;
		}
		else if (direction == Direction::kLeft) {
			direction = Direction::kLeftStand;
		}

		if (isJump) {
			velocity_ = velocity_ + kAcceleration_;

			//最高速度を制限
			if (kMaxFallSpeed >= velocity_.y) {
				velocity_.y = kMaxFallSpeed;
			}

			//地面についたら接地状態に切り替え
			if (velocity_.y < 0 && translation_.y <= nowGround_) {
				isGround_ = true;
				translation_.y = nowGround_;
				velocity_.y = 0;
			}
		}
	}

	//鍵を持った状態じゃないとゴールにしない
	if (!haveKey_) {
		isGoal_ = false;
	}

	//ScrollMove();
}

void PlayerTop::MoveResult()
{
	//前の座標を記憶
	preTranslation_ = translation_;

	translation_ = translation_ + velocity_;

}

void PlayerTop::Draw()
{
	//スクリーン座標に変換
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translation_);
	wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, camera_.worldMatrix, camera_.vertex, camera_.viewPortPosition, camera_.viewPortSize);


	screenPosition_ = Transform(initialPosition_, wvpVpMatrix_);

	Novice::DrawBox(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f), (int)kWidth_, (int)kHeight_, 0.0f, RED, kFillModeWireFrame);

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
		case kRightJump:
			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				(int)kWidth_ * (animationTimer / 7), 0, (int)kWidth_, (int)kHeight_, jumpRight,
				WHITE);
			break;
		case kLeftJump:
			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				(int)kWidth_ * (animationTimer / 7), 0, (int)kWidth_, (int)kHeight_, jumpLeft,
				WHITE);
			break;
		default:
			break;
	}
#ifdef _DEBUG
	ImGui::DragFloat2("PlayerGirl.Translation", &translation_.x, 0.01f);
	ImGui::DragFloat2("PlayerGirl.ScreenTranslation", &screenPosition_.x, 0.01f);
#endif // _DEBUG

}

void PlayerTop::Move()
{
	velocity_.x = 0.0f;

	isLanding_ = false;
	isCollideCeiling_ = false;
	isContactWall_ = false;
	isMagicCircleTouch_ = false;

	//当たり判定下
	MapCollisionBottom();

	//ジャンプ中
	//地面についていて上押されたらジャンプ開始
	if ((isGround_ || isLanding_) && (Novice::CheckHitKey(DIK_W) || Novice::CheckHitKey(DIK_UPARROW))) {
		isGround_ = false;
		isJump = true;

		velocity_.y = 10.0f;
	}

	if (isJump) {
		velocity_ = velocity_ + kAcceleration_;

		//最高速度を制限
		if (kMaxFallSpeed >= velocity_.y) {
			velocity_.y = kMaxFallSpeed;
		}

		//天井との当たり判定
		MapCollisionTop();
		//directionをジャンプに切り替え
		if (direction == Direction::kRight || direction == Direction::kRightStand) {
			direction = Direction::kRightJump;
		}
		else if (direction == Direction::kLeft || direction == Direction::kLeftStand) {
			direction = Direction::kLeftJump;
		}
		//地面についたら接地状態に切り替え
		if (velocity_.y < 0 && translation_.y <= nowGround_) {
			isGround_ = true;
			translation_.y = nowGround_;
			velocity_.y = 0;

		}
	}
	if (isLanding_ == true || isGround_ == true) {
		//directionを待機に切り替え
		if (direction == Direction::kRightJump) {
			direction = Direction::kRight;
		}
		else if (direction == Direction::kLeftJump) {
			direction = Direction::kLeft;
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

	////directionを待機に切り替え
	if (velocity_.x == 0) {
		if (direction == Direction::kRight) {

			direction = Direction::kRightStand;
		}
		else if (direction == Direction::kLeft) {
			direction = Direction::kLeftStand;
		}
	}

	if (isGround_) {
		velocity_.y = 0.0f;
		translation_.y = nowGround_;
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

	if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isCollideCeiling_ = true;
	}
	if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isCollideCeiling_ = true;
	}

	if (isKeyDraw_ == true) {
		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}
	}

	if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isGoal_ = true;
	}

	//右上
	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightTop] + Vector2{ -1.0f, 0.0f });

	if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isCollideCeiling_ = true;
	}
	if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isCollideCeiling_ = true;
	}

	if (isKeyDraw_ == true) {
		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}
	}

	if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isGoal_ = true;
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

	if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isLanding_ = true;
	}
	if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isLanding_ = true;
	}

	if (isKeyDraw_ == true) {
		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}
	}

	if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isGoal_ = true;
	}

	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector2{ 1.0f, -1.0f });

	if (MapChipType::kMagicCircle == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isMagicCircleTouch_ = true;
	}

	//右下
	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector2{ -1.0f, 0.0f });

	if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isLanding_ = true;
	}
	if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
		velocity_.y = 0.0f;
		isLanding_ = true;
	}

	if (isKeyDraw_ == true) {
		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}
	}

	if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isGoal_ = true;
	}

	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector2{ -1.0f, -1.0f });

	if (MapChipType::kMagicCircle == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isMagicCircleTouch_ = true;
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

		if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
		if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}

		if (isKeyDraw_ == true) {
			if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				haveKey_ = true;
			}
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal_ = true;
		}

	}

	//右真ん中
	IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(translation_ + velocity_ + Vector2{ kWidth_ / 2.0f, 0.0f });

	if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
		velocity_.x = 0.0f;
	}
	if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
		velocity_.x = 0.0f;
	}

	if (isKeyDraw_ == true) {
		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}
	}

	//ブロックが下にあったら当たり判定とらない
	if (!isLanding_) {
		//右下
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);

		if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
		if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}

		if (isKeyDraw_ == true) {
			if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				haveKey_ = true;
			}
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal_ = true;
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

		if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
		if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}

		if (isKeyDraw_ == true) {
			if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				haveKey_ = true;
			}
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal_ = true;
		}

	}

	//左真ん中
	IndexSet indexSet = mapChipManager_->GetMapChipIndexSetByPosition(translation_ + velocity_ - Vector2{ kWidth_ / 2.0f, 0.0f });

	if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
		velocity_.x = 0.0f;
	}

	if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
		velocity_.x = 0.0f;
	}

	//ブロックが下にあったら当たり判定とらない
	if (!isLanding_) {
		//左下
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);

		if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
		if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
		if (isKeyDraw_ == true) {
			if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				haveKey_ = true;
			}
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal_ = true;
		}

	}

}

void PlayerTop::SetMapChipField(MapChipManager* mapChipManager)
{
	mapChipManager_ = mapChipManager;
}

void PlayerTop::AnimationTimerChange()
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
		case kRightJump:
			animationTimerReset = 56;
			break;
		case kLeftJump:
			animationTimerReset = 56;
			break;

		default:
			break;
	}

	if (animationTimer >= animationTimerReset) {
		animationTimer = 0;
	}
}

void PlayerTop::SwapTopBottom()
{
	//地面座標入れ替え
	if (nowGround_ == kTopGround_) {
		nowGround_ = kBottomGround_;
	}
	else {
		nowGround_ = kTopGround_;
	}

	//座標入れ替え
	translation_.y = nowGround_;
}

void PlayerTop::ScrollMove()
{
	if (translation_.x >= kWindowWidth / 2.0f) {
		camera_.viewPortPosition.x = translation_.x - kWindowWidth;
	}
}

void PlayerTop::SetCamera(Camera camera)
{
	camera_ = camera;
}

Camera PlayerTop::GetCamera()
{
	return camera_;
}

bool PlayerTop::IsGoal()
{
	return isGoal_;
}

bool PlayerTop::HaveKey() {
	return haveKey_;
}

void PlayerTop::IsKeyDraw(bool isKeyDraw)
{
	isKeyDraw_ = isKeyDraw;
}

bool PlayerTop::IsMagicCircleTouch()
{
	return isMagicCircleTouch_;
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
