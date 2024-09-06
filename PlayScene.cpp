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
}

void PlayScene::PlayerBottomMoveUpdate()
{
	// キー入力を受け取る
	memcpy(preKeys, keys, 256);
	Novice::GetHitKeyStateAll(keys);

	switch (phase) {
		case Phase::kMovePlayerTop:

			//プレイヤーの更新
			playerTop_->PlayerTopMoveUpdate();
			playerBottom_->PlayerBottomMoveUpdate();

			//フェーズを変える
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				phase = Phase::kMovePlayerBottom;
			}

			break;

		case Phase::kMovePlayerBottom:

			//プレイヤーの更新
			playerTop_->PlayerBottomMoveUpdate();
			playerBottom_->PlayerBottomMoveUpdate();

			//フェーズを変える
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				phase = Phase::kMovePlayerTop;
			}

			break;

		case Phase::kMoveAll:
			//プレイヤーの更新
			playerTop_->PlayerBottomMoveUpdate();
			playerBottom_->PlayerBottomMoveUpdate();

			break;
	}

	//プレイヤーの更新
	playerTop_->PlayerBottomMoveUpdate();
	playerBottom_->PlayerBottomMoveUpdate();


	//当たり判定をとる
	CheckCollision();

	//if (Novice::CheckHitKey(DIK_SPACE)) {
	//	sceneNo = kClear;
	//}



}

void PlayScene::Draw()
{

	//プレイヤーの描画
	playerTop_->Draw(cameraManager_->GetCamera());
	playerBottom_->Draw(cameraManager_->GetCamera());


	//マップの描画
	DrawMap();

}

void PlayScene::CheckCollision()
{
	for (uint32_t i = 0; i < mapChipField_->GetBlockNum(); i++) {
		//下のプレイヤーとブロックの当たり判定
		if (isCollision(playerBottom_->GetAABB(), block[i].aabb_)) {
			playerBottom_->OnCollision();

			//ブロックを移動させる
			block[i].velocity.x += playerBottom_->GetVelocity().x;

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
		for (uint32_t j = 0; j < numBlockHorizonal;j++) {

			//スクリーン座標に変換
			worldMatrix_ = MakeAffineMatrix({ 1.0f, 1.0f }, 0.0f, { j * kBlockWidth, i * kBlockHeight });
			wvpVpMatrix_ = MakewvpVpMatrix(worldMatrix_, cameraManager_->GetCamera().worldMatrix, cameraManager_->GetCamera().vertex, cameraManager_->GetCamera().viewPortPosition, cameraManager_->GetCamera().viewPortSize);

			screenPosition_ = Transform({ 0, 0 }, wvpVpMatrix_);

			switch (mapChipField_->GetMapChipDate().data[i][j]) {
				case MapChipType::kBlank:
					break;

				case MapChipType::kGround_:
					Novice::DrawBox((int)(screenPosition_.x - kBlockWidth / 2), (int)(screenPosition_.y - kBlockWidth / 2), (int)kBlockWidth, (int)kBlockHeight, 0.0f, GREEN, kFillModeSolid);
					break;

				case MapChipType::kBlock:
					//ブロックのaabbを計算
					block[k].aabb_.max = { j * kBlockWidth + block[k].velocity.x + kBlockWidth / 2, i * kBlockHeight + block[k].velocity.y + kBlockHeight / 2 };
					block[k].aabb_.min = { j * kBlockWidth + block[k].velocity.x - kBlockWidth / 2, i * kBlockHeight + block[k].velocity.y - kBlockHeight / 2 };

					Novice::DrawBox((int)(screenPosition_.x + block[k].velocity.x - kBlockWidth / 2), (int)(screenPosition_.y - kBlockHeight / 2), (int)kBlockWidth, (int)kBlockHeight, 0.0f, BLACK, kFillModeSolid);
					
					k++;

					break;
			}

		}
	}
}
