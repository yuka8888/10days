#include "PlayScene.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
	delete playerGirl_;
	delete playerBoy_;
	delete cameraManager_;
}

void PlayScene::Initialize()
{
	//プレイヤーの初期化
	playerGirl_ = new PlayerTop;
	playerGirl_->Initialize();
	playerBoy_ = new PlayerBottom;
	playerBoy_->Initialize();

	//カメラの初期化
	cameraManager_ = new CameraManager;
	cameraManager_->Initialize();

	// マップチップ
	mapChipField_ = new MapChipManager;

	//ステージ読み込み
	stageNo_++;
	switch (stageNo_)
	{
		case 0:
			mapChipField_->LoadMapChipCsv("Resources/map.csv");
			break;
		case 1:
			mapChipField_->LoadMapChipCsv("Resources/map.csv");
			break;
		case 2:
			mapChipField_->LoadMapChipCsv("Resources/map2.csv");
			break;
		default:
			break;
	}

	isKeyDraw_ = false;
	isStageClear = false;

	//ブロックの情報を入れる
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizonal = mapChipField_->GetNumBlockHorizontal();

	uint32_t k = 0;
	uint32_t l = 0;
	uint32_t m = 0;

	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizonal; j++) {

			switch (mapChipField_->GetMapChipDate().data[i][j]) {
				case MapChipType::kBlockBottom:

					//ブロックの初期位置を取得
					blockBottom_[k].initialPosition = { j * kBlockWidth_, i * kBlockHeight_ };

					//ブロックのaabbを計算
					blockBottom_[k].aabb_.max = { j * kBlockWidth_ + blockBottom_[k].velocity.x + kBlockWidth_ / 2, i * kBlockHeight_ + blockBottom_[k].velocity.y + kBlockHeight_ / 2 };
					blockBottom_[k].aabb_.min = { j * kBlockWidth_ + blockBottom_[k].velocity.x - kBlockWidth_ / 2, i * kBlockHeight_ + blockBottom_[k].velocity.y - kBlockHeight_ / 2 };

					k++;

					break;

				case MapChipType::kBlockTop:
					//ブロックの初期位置を取得
					blockTop_[m].initialPosition = { j * kBlockWidth_, i * kBlockHeight_ };

					//ブロックのaabbを計算
					blockTop_[m].aabb_.max = { j * kBlockWidth_ + blockTop_[m].velocity.x + kBlockWidth_ / 2, i * kBlockHeight_ + blockTop_[m].velocity.y + kBlockHeight_ / 2 };
					blockTop_[m].aabb_.min = { j * kBlockWidth_ + blockTop_[m].velocity.x - kBlockWidth_ / 2, i * kBlockHeight_ + blockTop_[m].velocity.y - kBlockHeight_ / 2 };

					m++;

					break;

				case MapChipType::kFall:
					//落とし穴のaabbを計算
					fallAABB_[l].max = { j * kBlockWidth_ + kBlockWidth_ / 2, i * kBlockHeight_ + kBlockHeight_ / 2 };
					fallAABB_[l].min = { j * kBlockWidth_ - kBlockWidth_ / 2, i * kBlockHeight_ - kBlockHeight_ / 2 };

					l++;
			}

		}
	}

	//マップ生成
	DrawMap();

	playerGirl_->SetTopBlockNum(mapChipField_->GetBlockTopNum());
	playerGirl_->SetBottomBlockNum(mapChipField_->GetBlockBottomNum());

	//ブロックを共有
	playerGirl_->SetBlockTop(blockTop_);
	playerGirl_->SetBlockBottom(blockBottom_);

	//マップチップの読み込み
	playerGirl_->SetMapChipField(mapChipField_);
	playerBoy_->SetMapChipField(mapChipField_);

}

void PlayScene::Update()
{
	// キー入力を受け取る
	memcpy(preKeys, keys, 256);
	Novice::GetHitKeyStateAll(keys);

	playerGirl_->SetCamera(cameraManager_->GetCamera());

	//背景
	backAnimationTimer++;
	if (backAnimationTimer >= backAnimationTimerReset)
	{
		backAnimationTimer = 0;
	}

	//ブロックを共有
	playerGirl_->SetBlockTop(blockTop_);
	playerGirl_->SetBlockBottom(blockBottom_);

	switch (phase) {
		case Phase::kMovePlayerGirlTop:

			//アニメーションタイマーの更新
			animationTimer++;
			//アニメーションタイマーのリセット
			if (animationTimer >= animationTimerReset) {
				animationTimer = 0;
			}

			//プレイヤーの更新
			playerGirl_->PlayerMovePhaseUpdate();
			playerBoy_->PlayerStopPhaseUpdate();

			//スクロール
			for (int i = 0; i < 8; ++i)
			{
				backLocalPos[i] = int(backTreePos[i] + playerGirl_->GetBackTreeScroll());
			}
			for (int i = 0; i < 8; ++i)
			{
				frontLocalPos[i] = int(frontTreePos[i] + playerGirl_->GetFrontTreeScroll());
			}

			//フェーズを変える
			//魔法陣に触れていたら上下の移動
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] && playerGirl_->IsMagicCircleTouch() && playerBoy_->IsMagicCircleTouch()) {
				phase = Phase::kMovePlayerGirlBottom;
				//座標入れ替え
				playerGirl_->SwapTopBottom();
				playerBoy_->SwapTopBottom();

				//スクロールの値の共有
				playerBoy_->SetFrontTreeScroll(playerGirl_->GetFrontTreeScroll());
				playerBoy_->SetBackTreeScroll(playerGirl_->GetBackTreeScroll());
			}
			else if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				phase = Phase::kMovePlayerBoyBottom;
			}


			break;

		case Phase::kMovePlayerBoyTop:

			//アニメーションタイマーの更新
			animationTimer++;
			//アニメーションタイマーのリセット
			if (animationTimer >= animationTimerReset) {
				animationTimer = 0;
			}

			//プレイヤーの更新
			playerGirl_->PlayerStopPhaseUpdate();
			playerBoy_->PlayerMovePhaseUpdate();

			//スクロール
			for (int i = 0; i < 8; ++i)
			{
				backLocalPos[i] = int(backTreePos[i] + playerBoy_->GetBackTreeScroll());
			}
			for (int i = 0; i < 8; ++i)
			{
				frontLocalPos[i] = int(frontTreePos[i] + playerBoy_->GetFrontTreeScroll());
			}

			//フェーズを変える
			//魔法陣に触れていたら上下の移動
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] && playerGirl_->IsMagicCircleTouch() && playerBoy_->IsMagicCircleTouch()) {
				phase = Phase::kMovePlayerBoyBottom;
				//座標入れ替え
				playerGirl_->SwapTopBottom();
				playerBoy_->SwapTopBottom();

				//スクロールの値の共有
				playerGirl_->SetFrontTreeScroll(playerBoy_->GetFrontTreeScroll());
				playerGirl_->SetBackTreeScroll(playerBoy_->GetBackTreeScroll());

			}
			else if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				phase = Phase::kMovePlayerGirlTop;
			}

			break;

		case Phase::kMovePlayerGirlBottom:


			//アニメーションタイマーの更新
			animationTimer++;
			//アニメーションタイマーのリセット
			if (animationTimer >= animationTimerReset) {
				animationTimer = 0;
			}

			//アニメーションタイマーの更新
			animationTimer++;
			//アニメーションタイマーのリセット
			if (animationTimer >= animationTimerReset) {
				animationTimer = 0;
			}

			//プレイヤーの更新
			playerGirl_->PlayerMovePhaseUpdate();
			playerBoy_->PlayerStopPhaseUpdate();

			//フェーズを変える
			//魔法陣に触れていたら上下の移動
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] && playerGirl_->IsMagicCircleTouch() && playerBoy_->IsMagicCircleTouch()) {
				phase = Phase::kMovePlayerGirlTop;
				//座標入れ替え
				playerGirl_->SwapTopBottom();
				playerBoy_->SwapTopBottom();

				//スクロールの値の共有
				playerGirl_->SetFrontTreeScroll(playerBoy_->GetFrontTreeScroll());
				playerGirl_->SetBackTreeScroll(playerBoy_->GetBackTreeScroll());

			}
			else if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				phase = Phase::kMovePlayerBoyTop;
			}

			break;

		case Phase::kMovePlayerBoyBottom:

			//アニメーションタイマーの更新
			animationTimer++;
			//アニメーションタイマーのリセット
			if (animationTimer >= animationTimerReset) {
				animationTimer = 0;
			}

			//アニメーションタイマーの更新
			animationTimer++;
			//アニメーションタイマーのリセット
			if (animationTimer >= animationTimerReset) {
				animationTimer = 0;
			}

			//プレイヤーの更新
			playerBoy_->PlayerMovePhaseUpdate();
			playerGirl_->PlayerStopPhaseUpdate();

			//フェーズを変える
			//魔法陣に触れていたら上下の移動
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] && playerGirl_->IsMagicCircleTouch() && playerBoy_->IsMagicCircleTouch()) {
				phase = Phase::kMovePlayerBoyTop;
				//座標入れ替え
				playerGirl_->SwapTopBottom();
				playerBoy_->SwapTopBottom();

				//スクロールの値の共有
				playerBoy_->SetFrontTreeScroll(playerGirl_->GetFrontTreeScroll());
				playerBoy_->SetBackTreeScroll(playerGirl_->GetBackTreeScroll());

			}
			else if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				phase = Phase::kMovePlayerGirlTop;
			}

			break;
	}

	//当たり判定をとる
	CheckCollision();

	//ボタンを押しているか共有

	//鍵をとっているか共有
	playerBoy_->HaveKey(playerGirl_->HaveKey());

	if (playerGirl_->IsGoal() && playerBoy_->IsGoal()) {
		isStageClear = true;
	}

	//プレイヤーを最終的に移動させる
	playerGirl_->MoveResult();
	playerBoy_->MoveResult();

	cameraManager_->SetViewPortPosition(playerGirl_->GetCamera().viewPortPosition);

}

void PlayScene::Draw()
{
	//背景の描画
	//上
	//奥の草
	Novice::DrawSprite(0, 0, farGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	Novice::DrawSprite(kWidth_, 0, farGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	//後ろの木
	for (int i = 0; i < 8; ++i)
	{
		Novice::DrawSprite(backLocalPos[i], 0, backTreeTexture, 1.0f, 1.0f, 0.0f, WHITE);
	}
	//真ん中の草
	Novice::DrawSprite(0, 0, centerGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	Novice::DrawSprite(kWidth_, 0, centerGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	//前の木
	for (int i = 0; i < 8; ++i)
	{
		Novice::DrawSprite(frontLocalPos[i], 0, frontTreeTexture, 1.0f, 1.0f, 0.0f, WHITE);
	}
	//前の草
	Novice::DrawSprite(0, 0, frontGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	Novice::DrawSprite(kWidth_, 0, frontGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);

	//下
	Novice::DrawQuad(0, int(kBackPos_.y), int(kBackPos_.x), int(kBackPos_.y), 0, kWidth_, int(kBackPos_.x), kWidth_, kWidth_ * (backAnimationTimer / 7), 0, kWidth_, kHeight_, bg_underTexture, WHITE);
	Novice::DrawQuad(int(kBackPos_.x), int(kBackPos_.y), kMostPos_, int(kBackPos_.y), int(kBackPos_.x), kWidth_, kMostPos_, kWidth_, kWidth_ * (backAnimationTimer / 7), 0, kWidth_, kHeight_, bg_underTexture, WHITE);


	//プレイヤーの描画
	playerGirl_->Draw();
	playerBoy_->Draw(cameraManager_->GetCamera());


	//マップの描画
	DrawMap();

}

void PlayScene::CheckCollision()
{
	isPreBlockAndPlayerBottomCollision = isBlockAndPlayerBottomCollision_;
	isBlockAndPlayerBottomCollision_ = false;
	for (uint32_t i = 0; i < mapChipField_->GetBlockTopNum(); i++) {
		//下のプレイヤーとブロックの衝突判定
		if (isCollision(playerBoy_->GetAABB(), blockTop_[i].aabb_) && (playerBoy_->GetDirection() == Direction::kRight || playerBoy_->GetDirection() == Direction::kRightStand) && !blockTop_[i].isFall) {
			isBlockAndPlayerBottomCollision_ = true;

			//もし最初に触れるならブロックにくっつけるように移動
			if (isPreBlockAndPlayerBottomCollision == false && isBlockAndPlayerBottomCollision_ == true) {
				playerBoy_->SetTranslation({ blockTop_[i].aabb_.min.x - playerBoy_->GetSize().x / 2.0f, playerBoy_->GetTranslation().y });
			}

			playerBoy_->OnCollision();

			//ブロックを移動させる
			blockTop_[i].velocity.x = (playerBoy_->GetTranslation().x - blockTop_[i].initialPosition.x + kBlockWidth_ / 2 + playerBoy_->GetSize().x / 2);

			//ブロックのaabbの計算しなおし
			blockTop_[i].aabb_.max = { blockTop_[i].initialPosition.x + blockTop_[i].velocity.x + kBlockWidth_ / 2, blockTop_[i].initialPosition.y + blockTop_[i].velocity.y + kBlockHeight_ / 2 };
			blockTop_[i].aabb_.min = { blockTop_[i].initialPosition.x + blockTop_[i].velocity.x - kBlockWidth_ / 2, blockTop_[i].initialPosition.y + blockTop_[i].velocity.y - kBlockHeight_ / 2 };

		}
	}

	for (uint32_t i = 0; i < mapChipField_->GetBlockBottomNum(); i++) {
		//下のプレイヤーとブロックの衝突判定
		if (isCollision(playerBoy_->GetAABB(), blockBottom_[i].aabb_) && (playerBoy_->GetDirection() == Direction::kRight || playerBoy_->GetDirection() == Direction::kRightStand) && !blockBottom_[i].isFall) {
			isBlockAndPlayerBottomCollision_ = true;

			//もし最初に触れるならブロックにくっつけるように移動
			if (isPreBlockAndPlayerBottomCollision == false && isBlockAndPlayerBottomCollision_ == true) {
				playerBoy_->SetTranslation({ blockBottom_[i].aabb_.min.x - playerBoy_->GetSize().x / 2.0f, playerBoy_->GetTranslation().y });
			}

			playerBoy_->OnCollision();

			//ブロックを移動させる
			blockBottom_[i].velocity.x = (playerBoy_->GetTranslation().x - blockBottom_[i].initialPosition.x + kBlockWidth_ / 2 + playerBoy_->GetSize().x / 2);

			//ブロックのaabbの計算しなおし
			blockBottom_[i].aabb_.max = { blockBottom_[i].initialPosition.x + blockBottom_[i].velocity.x + kBlockWidth_ / 2, blockBottom_[i].initialPosition.y + blockBottom_[i].velocity.y + kBlockHeight_ / 2 };
			blockBottom_[i].aabb_.min = { blockBottom_[i].initialPosition.x + blockBottom_[i].velocity.x - kBlockWidth_ / 2, blockBottom_[i].initialPosition.y + blockBottom_[i].velocity.y - kBlockHeight_ / 2 };

			////ブロック同士の衝突判定
			//for (uint32_t j = 0; j < mapChipField_->GetBlockNum(); j++) {
			//	if (i == j) {
			//	}
			//	else if (isCollision(block[i].aabb_, block[j].aabb_)) {
			//		block[i].velocity.x = block[j].initialPosition.x + block[j].velocity.x - kBlockWidth_ - block[i].initialPosition.x;
			//		//プレイヤーの位置をブロックと合わせる
			//		playerBoy_->PushTwoBlocks(block[i]);
			//	}
			//}
		}

		//ブロックと落とし穴の当たり判定
		for (uint32_t j = 0; j < mapChipField_->GetFallNum(); j++) {
			if (fallAABB_[j].max.x >= blockBottom_[i].aabb_.max.x - 0.5f && fallAABB_[j].max.x <= blockBottom_[i].aabb_.max.x + 0.5f && !blockBottom_[i].isFall) {
				blockBottom_[i].isFall = true;
				blockBottom_[i].velocity.x = (fallAABB_[j].min.x - blockBottom_[i].initialPosition.x + kBlockHeight_ / 2);
				startBlockPosition = blockBottom_[i].initialPosition + blockBottom_[i].velocity;
				endBlockPosition = { startBlockPosition.x, startBlockPosition.y - kBlockHeight_ };
			}
		}

		//ブロックが落下するとき
		if (blockBottom_[i].isFall) {
			blockFallTimer += 0.02f;

			if (blockFallTimer >= 1.0f) {
				blockFallTimer = 1.0f;
				playerBoy_->SetFallBlockIndex(mapChipField_->GetMapChipIndexSetByPosition(endBlockPosition));
				isKeyDraw_ = true;
				playerGirl_->IsKeyDraw(isKeyDraw_);
			}
			//線形補間でブロック落下
			blockBottom_[i].velocity.y = Lerp(startBlockPosition.y, endBlockPosition.y, easeInCubic(blockFallTimer));
		}
	}

}

void PlayScene::DrawMap()
{
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizonal = mapChipField_->GetNumBlockHorizontal();

	uint32_t k = 0;
	uint32_t l = 0;

	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizonal; j++) {

			//スクリーン座標に変換
			worldMatrix_ = MakeAffineMatrix({ 1.0f, 1.0f }, 0.0f, { j * kBlockWidth_, i * kBlockHeight_ });
			wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, cameraManager_->GetCamera().worldMatrix, cameraManager_->GetCamera().vertex, cameraManager_->GetCamera().viewPortPosition, cameraManager_->GetCamera().viewPortSize);

			screenPosition_ = Transform({ 0, 0 }, wvpVpMatrix_);

			switch (mapChipField_->GetMapChipDate().data[i][j]) {
				case MapChipType::kBlank:
					break;

				case MapChipType::kGround_:
					Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, GREEN, kFillModeSolid);
					break;

				case MapChipType::kBlockTop:
					//ブロックのaabbを計算
					blockTop_[k].aabb_.max = { j * kBlockWidth_ + blockTop_[k].velocity.x + kBlockWidth_ / 2, i * kBlockHeight_ + blockTop_[k].velocity.y + kBlockHeight_ / 2 };
					blockTop_[k].aabb_.min = { j * kBlockWidth_ + blockTop_[k].velocity.x - kBlockWidth_ / 2, i * kBlockHeight_ + blockTop_[k].velocity.y - kBlockHeight_ / 2 };

					Novice::DrawBox((int)(screenPosition_.x + blockTop_[k].velocity.x - kBlockWidth_ / 2), (int)(screenPosition_.y + blockTop_[k].velocity.y - kBlockHeight_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLACK, kFillModeSolid);
					Novice::DrawSprite((int)(screenPosition_.x + blockTop_[k].velocity.x - kBlockWidth_ / 2), (int)(screenPosition_.y + blockTop_[k].velocity.y - kBlockHeight_ / 2), blockTexture, 1.0f, 1.0f, 0.0f, WHITE);
					k++;

					break;

				case MapChipType::kBlockBottom:
					//ブロックのaabbを計算
					blockBottom_[l].aabb_.max = { j * kBlockWidth_ + blockBottom_[l].velocity.x + kBlockWidth_ / 2, i * kBlockHeight_ + blockBottom_[l].velocity.y + kBlockHeight_ / 2 };
					blockBottom_[l].aabb_.min = { j * kBlockWidth_ + blockBottom_[l].velocity.x - kBlockWidth_ / 2, i * kBlockHeight_ + blockBottom_[l].velocity.y - kBlockHeight_ / 2 };

					Novice::DrawBox((int)(screenPosition_.x + blockBottom_[l].velocity.x - kBlockWidth_ / 2), (int)(screenPosition_.y + blockBottom_[l].velocity.y - kBlockHeight_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLACK, kFillModeSolid);
					Novice::DrawSprite((int)(screenPosition_.x + blockBottom_[l].velocity.x - kBlockWidth_ / 2), (int)(screenPosition_.y + blockBottom_[l].velocity.y - kBlockHeight_ / 2), blockTexture, 1.0f, 1.0f, 0.0f, WHITE);
					l++;

					break;

				case MapChipType::kKey:
					if (isKeyDraw_) {
						if (!playerGirl_->HaveKey()) {
							Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLUE, kFillModeSolid);
							Novice::DrawQuad((int)(screenPosition_.x - 16 / 2), (int)(screenPosition_.y - 16 / 2),
								int(screenPosition_.x + 16 / 2.0f), int(screenPosition_.y - 16 / 2.0f),
								int(screenPosition_.x - 16 / 2.0f), int(screenPosition_.y + 16 / 2.0f),
								int(screenPosition_.x + 16 / 2.0f), int(screenPosition_.y + 16 / 2.0f),
								(int)16 * (animationTimer / 20), 0, (int)16, (int)16, keyTexture,
								WHITE);
						}
						else {

						}
					}

					break;

				case MapChipType::kGoal:
					if (!playerGirl_->HaveKey()) {
						Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, RED, kFillModeSolid);
						Novice::DrawSprite((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2) - 8, goalCloseTexture, 1.0f, 1.0f, 0.0f, WHITE);
					}
					else {
						Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLUE, kFillModeSolid);
						Novice::DrawSprite((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2) - 8, goalOpenTexture, 1.0f, 1.0f, 0.0f, WHITE);
					}
					break;

				case MapChipType::kFall:
					break;

				case MapChipType::kMagicCircle:

					Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockHeight_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, RED, kFillModeSolid);
					break;

				case MapChipType::kWall:
					//ブロックのaabbを計算

					Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockHeight_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLACK, kFillModeSolid);
					break;

				case MapChipType::kSwitch:

					Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockHeight_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLUE, kFillModeSolid);
					break;

			}

		}
	}


}
