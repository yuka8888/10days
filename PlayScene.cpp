#include "PlayScene.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
	delete playerTop_;
	delete playerBottom_;
	delete cameraManager_;
}

void PlayScene::Initialize()
{
	//プレイヤーの初期化
	playerTop_ = new PlayerTop;
	playerTop_->Initialize();
	playerBottom_ = new PlayerBottom;
	playerBottom_->Initialize();

	//カメラの初期化
	cameraManager_ = new CameraManager;
	cameraManager_->Initialize();

	// マップチップ
	mapChipField_ = new MapChipManager;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");


	//ブロックの情報を入れる
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizonal = mapChipField_->GetNumBlockHorizontal();

	uint32_t k = 0;


	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizonal; j++) {

			switch (mapChipField_->GetMapChipDate().data[i][j]) {
				case MapChipType::kBlock:
					//ブロックの初期位置を取得
					block[k].initialPosition = { j * kBlockWidth_ + block[k].velocity.x, i * kBlockHeight_ + block[k].velocity.y };
					
					//ブロックのaabbを計算
					block[k].aabb_.max = { j * kBlockWidth_ + block[k].velocity.x + kBlockWidth_ / 2, i * kBlockHeight_ + block[k].velocity.y + kBlockHeight_ / 2 };
					block[k].aabb_.min = { j * kBlockWidth_ + block[k].velocity.x - kBlockWidth_ / 2, i * kBlockHeight_ + block[k].velocity.y - kBlockHeight_ / 2 };

					Novice::DrawBox((int)(screenPosition_.x + block[k].velocity.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockHeight_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLACK, kFillModeSolid);

					k++;

					break;
			}

		}
	}

	//マップチップの読み込み
	playerTop_->SetMapChipField(mapChipField_);
	playerBottom_->SetMapChipField(mapChipField_);

}

void PlayScene::Update()
{
	// キー入力を受け取る
	memcpy(preKeys, keys, 256);
	Novice::GetHitKeyStateAll(keys);

	playerTop_->SetCamera(cameraManager_->GetCamera());

	switch (phase) {
		case Phase::kMovePlayerTop:
			//プレイヤーの更新
			playerTop_->PlayerTopPhaseUpdate();
			playerBottom_->PlayerTopPhaseUpdate();

			//フェーズを変える
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				phase = Phase::kMovePlayerBottom;
			}

			break;

		case Phase::kMovePlayerBottom:
			//プレイヤーの更新
			playerBottom_->PlayerBottomPhaseUpdate();
			playerTop_->PlayerBottomPhaseUpdate();

			//フェーズを変える
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				phase = Phase::kMovePlayerTop;
			}

			break;

		case Phase::kMoveAll:
			//プレイヤーの更新
			playerTop_->PlayerTopPhaseUpdate();
			playerBottom_->PlayerBottomPhaseUpdate();

			break;
	}

	//当たり判定をとる
	CheckCollision();

	//鍵をとっているか共有
	playerBottom_->HaveKey(playerTop_->HaveKey());

	//プレイヤーを最終的に移動させる
	playerTop_->MoveResult();
	playerBottom_->MoveResult();

	cameraManager_->SetViewPortPosition(playerTop_->GetCamera().viewPortPosition);

}

void PlayScene::Draw()
{

	//プレイヤーの描画
	playerTop_->Draw();
	playerBottom_->Draw(cameraManager_->GetCamera());


	//マップの描画
	DrawMap();

}

void PlayScene::CheckCollision()
{
	isPreBlockAndPlayerBottomCollision = isBlockAndPlayerBottomCollision_;
	isBlockAndPlayerBottomCollision_ = false;
	for (uint32_t i = 0; i < mapChipField_->GetBlockNum(); i++) {
		//下のプレイヤーとブロックの衝突判定
		if (isCollision(playerBottom_->GetAABB(), block[i].aabb_) && playerBottom_->GetDirection() == Direction::kRight) {
			isBlockAndPlayerBottomCollision_ = true;

			//もし最初に触れるならブロックにくっつけるように移動
			if (isPreBlockAndPlayerBottomCollision == false && isBlockAndPlayerBottomCollision_ == true) {
				playerBottom_->SetTranslation({ block[i].aabb_.min.x - playerBottom_->GetSize().x / 2.0f, playerBottom_->GetTranslation().y});
			}

			playerBottom_->OnCollision();

			//ブロックを移動させる
			block[i].velocity.x = (playerBottom_->GetTranslation().x - block[i].initialPosition.x + kBlockWidth_ / 2 + playerBottom_->GetSize().x / 2);

			//ブロックのaabbの計算しなおし
			block[i].aabb_.max = { block[i].initialPosition.x + block[i].velocity.x + kBlockWidth_ / 2, block[i].initialPosition.y + block[i].velocity.y + kBlockHeight_ / 2 };
			block[i].aabb_.min = { block[i].initialPosition.x + block[i].velocity.x - kBlockWidth_ / 2, block[i].initialPosition.y + block[i].velocity.y - kBlockHeight_ / 2 };

			//ブロック同士の衝突判定
			for (uint32_t j = 0; j < mapChipField_->GetBlockNum(); j++) {
				if (i == j) {
				}
				else if (isCollision(block[i].aabb_, block[j].aabb_)) {
					block[i].velocity.x = block[j].initialPosition.x + block[j].velocity.x - kBlockWidth_ - block[i].initialPosition.x;
					//プレイヤーの位置をブロックと合わせる
					playerBottom_->PushTwoBlocks(block[i]);
				}
			}

		}

	}

}

void PlayScene::DrawMap()
{
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizonal = mapChipField_->GetNumBlockHorizontal();

	uint32_t k = 0;

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

				case MapChipType::kBlock:
					//ブロックのaabbを計算
					block[k].aabb_.max = { j * kBlockWidth_ + block[k].velocity.x + kBlockWidth_ / 2, i * kBlockHeight_ + block[k].velocity.y + kBlockHeight_ / 2 };
					block[k].aabb_.min = { j * kBlockWidth_ + block[k].velocity.x - kBlockWidth_ / 2, i * kBlockHeight_ + block[k].velocity.y - kBlockHeight_ / 2 };

					Novice::DrawBox((int)(screenPosition_.x + block[k].velocity.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockHeight_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLACK, kFillModeSolid);

					k++;

					break;

				case MapChipType::kKey:
					if (!playerTop_->HaveKey()) {
						Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLUE, kFillModeSolid);
					}
					else {

					}
					break;

				case MapChipType::kGoal:
					if (!playerTop_->HaveKey()) {
						Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, RED, kFillModeSolid);
					}
					else {
						Novice::DrawBox((int)(screenPosition_.x - kBlockWidth_ / 2), (int)(screenPosition_.y - kBlockWidth_ / 2), (int)kBlockWidth_, (int)kBlockHeight_, 0.0f, BLUE, kFillModeSolid);
					}
					break;
			}

		}
	}


}
