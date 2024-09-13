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
	isPressSwitch_ = false;
	velocity_.x = 0.0f;
	animationTimer++;
	if (isChanged_ == true) {
		changeAnimationTimer++;
	}
	else if (isChanged_ == false) {
		changeAnimationTimer = 0;
	}
	if (changeAnimationTimer >= changeAnimationTimerReset) {
		changeAnimationTimer = 0;
		isChanged_ = false;
	}
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
	if (isChanged_ == true) {
		changeAnimationTimer++;
	}
	else if (isChanged_ == false) {
		changeAnimationTimer = 0;
	}
	if (changeAnimationTimer >= changeAnimationTimerReset) {
		changeAnimationTimer = 0;
		isChanged_ = false;
	}


	if (!isGoal_) {
		Move();
	}
	else {

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

		if (changeAnimationTimer > 0) {
			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kChangeHeight / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kChangeHeight / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kChangeHeight / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kChangeHeight / 2.0f),
				(int)kWidth_ * (animationTimer / 7), 0, (int)kWidth_, (int)kChangeHeight, walkChangeRight,
				WHITE);
		}
		else {
			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				(int)kWidth_ * (animationTimer / 7), 0, (int)kWidth_, (int)kHeight_, walkRight,
				WHITE);

		}

		break;
	case kRightStand:
		if (changeAnimationTimer > 0) {
			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kChangeHeight / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kChangeHeight / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kChangeHeight / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kChangeHeight / 2.0f),
				(int)kWidth_ * (animationTimer / 30), 0, (int)kWidth_, (int)kChangeHeight, standingChangeRight,
				WHITE);
		}
		else {
			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				(int)kWidth_ * (animationTimer / 30), 0, (int)kWidth_, (int)kHeight_, standingRight,
				WHITE);
		}

		break;
	case kLeft:

		if (changeAnimationTimer > 0) {

			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kChangeHeight / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kChangeHeight / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kChangeHeight / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kChangeHeight / 2.0f),
				(int)kWidth_ * (animationTimer / 7), 0, (int)kWidth_, (int)kChangeHeight, walkChangeLeft,
				WHITE);
		}
		else {
			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				(int)kWidth_ * (animationTimer / 7), 0, (int)kWidth_, (int)kHeight_, walkLeft,
				WHITE);
		}


		break;
	case kLeftStand:
		if (changeAnimationTimer > 0) {

			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kChangeHeight / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kChangeHeight / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kChangeHeight / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kChangeHeight / 2.0f),
				(int)kWidth_ * (animationTimer / 30), 0, (int)kWidth_, (int)kChangeHeight, standingChangeLeft,
				WHITE);
		}
		else {
			Novice::DrawQuad(int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y - kHeight_ / 2.0f),
				int(screenPosition_.x - kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				int(screenPosition_.x + kWidth_ / 2.0f), int(screenPosition_.y + kHeight_ / 2.0f),
				(int)kWidth_ * (animationTimer / 30), 0, (int)kWidth_, (int)kHeight_, standingLeft,
				WHITE);
		}

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

	oldFrontTreeScroll_ = frontTreeScroll_;
	oldBackTreeScroll_ = backTreeScroll_;


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

	//当たり判定下
	MapCollisionBottom();

	//地面についていて上押されたらジャンプ開始
	if ((isGround_ || isLanding_) && (Novice::CheckHitKey(DIK_W) || Novice::CheckHitKey(DIK_UPARROW))) {
		isGround_ = false;
		isJump = true;

		velocity_.y = 10.0f;
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
		//スクロール
		frontTreeScroll_ += 0.2f;
		backTreeScroll_ += 0.05f;
		//当たり判定
		MapCollisionLeft();
	}
	else if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHTARROW)) {
		direction = Direction::kRight;
		velocity_.x = 2.0f;
		//スクロール
		frontTreeScroll_ -= 0.2f;
		backTreeScroll_ -= 0.05f;
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
		frontTreeScroll_ = oldFrontTreeScroll_;
		backTreeScroll_ = oldBackTreeScroll_;
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

	for (uint32_t i = 0; i < blockTopNum_; i++) {
		if (isCollision(blockTop_[i].aabb_, positionNew[kLeftTop] + Vector2{ 1.0f, 0.0f })) {
			translation_.y = blockTop_[i].initialPosition.y + blockTop_[i].velocity.y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isCollideCeiling_ = true;
		}
	}

	for (uint32_t i = 0; i < blockBottomNum_; i++) {
		if (isCollision(blockBottom_[i].aabb_, positionNew[kLeftTop] + Vector2{ 1.0f, 0.0f })) {
			translation_.y = blockBottom_[i].initialPosition.y + blockBottom_[i].velocity.y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isCollideCeiling_ = true;
		}
	}

	//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
	//	velocity_.y = 0.0f;
	//	isCollideCeiling_ = true;
	//}
	//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
	//	velocity_.y = 0.0f;
	//	isCollideCeiling_ = true;
	//}

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

	for (uint32_t i = 0; i < blockTopNum_; i++) {
		if (isCollision(blockTop_[i].aabb_, positionNew[kRightTop] + Vector2{ -1.0f, 0.0f })) {
			translation_.y = blockTop_[i].initialPosition.y + blockTop_[i].velocity.y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isCollideCeiling_ = true;
		}
	}

	for (uint32_t i = 0; i < blockBottomNum_; i++) {
		if (isCollision(blockBottom_[i].aabb_, positionNew[kRightTop] + Vector2{ -1.0f, 0.0f })) {
			translation_.y = blockBottom_[i].initialPosition.y + blockBottom_[i].velocity.y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isCollideCeiling_ = true;
		}
	}

	//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
	//	velocity_.y = 0.0f;
	//	isCollideCeiling_ = true;
	//}
	//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
	//	velocity_.y = 0.0f;
	//	isCollideCeiling_ = true;
	//}

	if (isKeyDraw_ == true) {
		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}
	}

	if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isGoal_ = true;
	}

	if (!isPressSwitch_ && isWallTopDraw_) {
		if (MapChipType::kWallTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isCollideCeiling_ = true;
		}
	}

	if (MapChipType::kSwitch == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isPressSwitch_ = true;
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

	for (uint32_t i = 0; i < blockTopNum_; i++) {
		if (isCollision(blockTop_[i].aabb_, positionNew[kLeftBottom])) {
			translation_.y = blockTop_[i].initialPosition.y + blockTop_[i].velocity.y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isLanding_ = true;
		}
	}

	for (uint32_t i = 0; i < blockBottomNum_; i++) {
		if (isCollision(blockBottom_[i].aabb_, positionNew[kLeftBottom] + Vector2{ 0.0f, -1.0f })) {
			translation_.y = blockBottom_[i].initialPosition.y + blockBottom_[i].velocity.y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isLanding_ = true;
		}
	}

	if (!isPressSwitch_) {
		if (MapChipType::kWallBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isCollideCeiling_ = true;
		}
	}

	if (!isPressSwitch_ && isWallTopDraw_) {
		if (MapChipType::kWallTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y - mapChipManager_->GetBlockSize().y / 2.0f - kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isCollideCeiling_ = true;
		}
	}

	if (MapChipType::kSwitch == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isPressSwitch_ = true;
	}

	//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
	//	velocity_.y = 0.0f;
	//	isLanding_ = true;
	//}
	//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
	//	velocity_.y = 0.0f;
	//	isLanding_ = true;
	//}

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

	for (uint32_t i = 0; i < blockTopNum_; i++) {
		if (isCollision(blockTop_[i].aabb_, positionNew[kRightBottom] + Vector2{ -1.0f, 0.0f })) {
			translation_.y = blockTop_[i].initialPosition.y + blockTop_[i].velocity.y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isLanding_ = true;
		}
	}
	//
	for (uint32_t i = 0; i < blockBottomNum_; i++) {
		if (isCollision(blockBottom_[i].aabb_, positionNew[kRightBottom] + Vector2{ 0.0f, -1.0f })) {
			translation_.y = blockBottom_[i].initialPosition.y + blockBottom_[i].velocity.y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isLanding_ = true;
		}
	}

	//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
	//	velocity_.y = 0.0f;
	//	isLanding_ = true;
	//}
	//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
	//	velocity_.y = 0.0f;
	//	isLanding_ = true;
	//}

	if (isKeyDraw_ == true) {
		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}
	}

	if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isGoal_ = true;
	}

	if (MapChipType::kSwitch == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isPressSwitch_ = true;
	}


	indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector2{ -1.0f, -1.0f });

	if (MapChipType::kMagicCircle == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isMagicCircleTouch_ = true;
	}

	if (!isPressSwitch_) {
		if (MapChipType::kWallBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isLanding_ = true;
		}
	}

	if (!isPressSwitch_ && isWallTopDraw_) {
		if (MapChipType::kWallTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			translation_.y = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).y + mapChipManager_->GetBlockSize().y / 2.0f + kHeight_ / 2.0f;
			velocity_.y = 0.0f;
			isLanding_ = true;
		}
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

		for (uint32_t i = 0; i < blockTopNum_; i++) {
			if (isCollision(blockTop_[i].aabb_, positionNew[kRightTop])) {
				translation_.x = blockTop_[i].initialPosition.x + blockTop_[i].velocity.x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
				velocity_.x = 0.0f;
			}
		}

		for (uint32_t i = 0; i < blockBottomNum_; i++) {
			if (isCollision(blockBottom_[i].aabb_, positionNew[kRightTop])) {
				translation_.x = blockBottom_[i].initialPosition.x + blockBottom_[i].velocity.x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
				velocity_.x = 0.0f;
			}
		}

		//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
		//	velocity_.x = 0.0f;
		//}
		//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
		//	velocity_.x = 0.0f;
		//}

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

	for (uint32_t i = 0; i < blockTopNum_; i++) {
		if (isCollision(blockTop_[i].aabb_, translation_ + velocity_ + Vector2{ kWidth_ / 2.0f, 0.0f })) {
			translation_.x = blockTop_[i].initialPosition.x + blockTop_[i].velocity.x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
	}

	for (uint32_t i = 0; i < blockBottomNum_; i++) {
		if (isCollision(blockBottom_[i].aabb_, translation_ + velocity_ + Vector2{ kWidth_ / 2.0f, 0.0f })) {
			translation_.x = blockBottom_[i].initialPosition.x + blockBottom_[i].velocity.x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
	}

	//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
	//	velocity_.x = 0.0f;
	//}
	//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
	//	velocity_.x = 0.0f;
	//}

	if (isKeyDraw_ == true) {
		if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			haveKey_ = true;
		}
	}

	if (MapChipType::kSwitch == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		isPressSwitch_ = true;
	}


	//ブロックが下にあったら当たり判定とらない
	if (!isLanding_) {
		//右下
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);

		for (uint32_t i = 0; i < blockTopNum_; i++) {
			if (isCollision(blockTop_[i].aabb_, positionNew[kRightBottom])) {
 				translation_.x = blockTop_[i].initialPosition.x + blockTop_[i].velocity.x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f - 1.0f;
				velocity_.x = 0.0f;
			}
		}

		for (uint32_t i = 0; i < blockBottomNum_; i++) {
			if (isCollision(blockBottom_[i].aabb_, positionNew[kRightBottom] + Vector2{0.0f, -1.0f})) {
				translation_.x = blockBottom_[i].initialPosition.x + blockBottom_[i].velocity.x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f -1.0f;
				velocity_.x = 0.0f;
			}
		}

		//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
		//	velocity_.x = 0.0f;
		//}
		//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
		//	velocity_.x = 0.0f;
		//}

		if (isKeyDraw_ == true) {
			if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				haveKey_ = true;
			}
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal_ = true;
		}

		if (!isPressSwitch_) {
			if (MapChipType::kWallBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
				velocity_.x = 0.0f;
			}
		}

		if (!isPressSwitch_ && isWallTopDraw_) {
			if (MapChipType::kWallTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x - mapChipManager_->GetBlockSize().x / 2.0f - kWidth_ / 2.0f;
				velocity_.x = 0.0f;
			}
		}

		if (MapChipType::kSwitch == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isPressSwitch_ = true;
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

		for (uint32_t i = 0; i < blockTopNum_; i++) {
			if (isCollision(blockTop_[i].aabb_, positionNew[kLeftTop])) {
				translation_.x = blockTop_[i].initialPosition.x + blockTop_[i].velocity.x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
				velocity_.x = 0.0f;
			}
		}

		for (uint32_t i = 0; i < blockBottomNum_; i++) {
			if (isCollision(blockBottom_[i].aabb_, positionNew[kLeftTop])) {
				translation_.x = blockBottom_[i].initialPosition.x + blockBottom_[i].velocity.x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
				velocity_.x = 0.0f;
			}
		}

		//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
		//	velocity_.x = 0.0f;
		//}
		//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
		//	velocity_.x = 0.0f;
		//}

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

	for (uint32_t i = 0; i < blockTopNum_; i++) {
		if (isCollision(blockTop_[i].aabb_, translation_ + velocity_ - Vector2{ kWidth_ / 2.0f, 0.0f })) {
			translation_.x = blockTop_[i].initialPosition.x + blockTop_[i].velocity.x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
	}

	for (uint32_t i = 0; i < blockBottomNum_; i++) {
		if (isCollision(blockBottom_[i].aabb_, translation_ + velocity_ - Vector2{ kWidth_ / 2.0f, 0.0f })) {
			translation_.x = blockBottom_[i].initialPosition.x + blockBottom_[i].velocity.x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
			velocity_.x = 0.0f;
		}
	}

	//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
	//	velocity_.x = 0.0f;
	//}

	//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
	//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
	//	velocity_.x = 0.0f;
	//}

	//ブロックが下にあったら当たり判定とらない
	if (!isLanding_) {
		//左下
		indexSet = mapChipManager_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
		for (uint32_t i = 0; i < blockTopNum_; i++) {
			if (isCollision(blockTop_[i].aabb_, positionNew[kLeftBottom])) {
				translation_.x = blockTop_[i].initialPosition.x + blockTop_[i].velocity.x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f + 2.0f;
				velocity_.x = 0.0f;
			}
		}

		for (uint32_t i = 0; i < blockBottomNum_; i++) {
			if (isCollision(blockBottom_[i].aabb_, positionNew[kLeftBottom])) {
				translation_.x = blockBottom_[i].initialPosition.x + blockBottom_[i].velocity.x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f + 2.0f;
				velocity_.x = 0.0f;
			}
		}
		//if (MapChipType::kBlockTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
		//	velocity_.x = 0.0f;
		//}
		//if (MapChipType::kBlockBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
		//	translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
		//	velocity_.x = 0.0f;
		//}
		if (isKeyDraw_ == true) {
			if (MapChipType::kKey == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				haveKey_ = true;
			}
		}

		if (MapChipType::kGoal == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isGoal_ = true;
		}

		if (!isPressSwitch_) {
			if (MapChipType::kWallBottom == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
				velocity_.x = 0.0f;
			}
		}

		if (!isPressSwitch_ && isWallTopDraw_) {
			if (MapChipType::kWallTop == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
				translation_.x = mapChipManager_->GetMapChipPositionByIndex(indexSet.xIndex, indexSet.yIndex).x + mapChipManager_->GetBlockSize().x / 2.0f + kWidth_ / 2.0f;
				velocity_.x = 0.0f;
			}
		}

		if (MapChipType::kSwitch == mapChipManager_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex)) {
			isPressSwitch_ = true;
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

void PlayerTop::SetBlockTop(Block* block)
{
	for (uint32_t i = 0; i < blockTopNum_; i++) {
		blockTop_[i].initialPosition = block[i].initialPosition;
		blockTop_[i].velocity = block[i].velocity;
		blockTop_[i].aabb_ = block[i].aabb_;
	}
}

void PlayerTop::SetBlockBottom(Block* block)
{
	for (uint32_t i = 0; i < blockBottomNum_; i++) {
		blockBottom_[i].initialPosition = block[i].initialPosition;
		blockBottom_[i].velocity = block[i].velocity;
		blockBottom_[i].aabb_ = block[i].aabb_;
	}

}

void PlayerTop::SetTopBlockNum(uint32_t blockNum)
{
	blockTopNum_ = blockNum;
}

void PlayerTop::SetBottomBlockNum(uint32_t blockNum)
{
	blockBottomNum_ = blockNum;
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

float PlayerTop::GetFrontTreeScroll()
{
	return frontTreeScroll_;
}

float PlayerTop::GetBackTreeScroll()
{
	return backTreeScroll_;
}

void PlayerTop::SetFrontTreeScroll(float frontTreeScroll)
{
	frontTreeScroll_ = frontTreeScroll;
}

void PlayerTop::SetBackTreeScroll(float backTreeScroll)
{
	backTreeScroll_ = backTreeScroll;
}

void PlayerTop::SetIsChanged(bool isChanged)
{
	isChanged_ = isChanged;
}

bool PlayerTop::HaveKey() {
	return haveKey_;
}

void PlayerTop::HaveKey(bool haveKey)
{
	haveKey_ = haveKey;
}

bool PlayerTop::IsPressSwitch()
{
	return isPressSwitch_;
}

void PlayerTop::IsPressSwitch(bool isPressSwitch)
{
	isPressSwitch_ = isPressSwitch;
}

void PlayerTop::IsKeyDraw(bool isKeyDraw)
{
	isKeyDraw_ = isKeyDraw;
}

bool PlayerTop::IsMagicCircleTouch()
{
	return isMagicCircleTouch_;
}

void PlayerTop::IsWallTopDraw(bool isWallTopDraw)
{
	isWallTopDraw_ = isWallTopDraw;
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
