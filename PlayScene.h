#pragma once
#include "IScene.h"
#include "Novice.h"
#include "CameraManager.h"
#include "PlayerTop.h"
#include "PlayerBottom.h"
#include "MapChipManager.h"

class PlayScene :public IScene
{
public:
	PlayScene();

	~PlayScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void CheckCollision();


	/// <summary>
	/// マップの生成
	/// </summary>
	void DrawMap();

private:
	//ゲームのフェーズ
	enum class Phase {
		kMovePlayerGirlTop, //女の子が上で動く
		kMovePlayerBoyTop, //男の子が上で動く
		kMovePlayerGirlBottom, //女の子が下で動く
		kMovePlayerBoyBottom, //男の子が下で動く
		kMoveAll, //どっちも動く
	};

	//現在のステージ
	uint32_t stageNo_ = 0;

	//現在のフェーズ
	Phase phase = Phase::kMovePlayerGirlTop;

	float kWindowHeight = 720.0f;
	float kWindowWidth = 1280.0f;

	//プレイヤー
	PlayerTop* playerGirl_ = nullptr;
	PlayerBottom* playerBoy_ = nullptr;

	//カメラの管理
	CameraManager* cameraManager_ = nullptr;

	//マップ
	MapChipManager* mapChipField_ = nullptr;

	//幅
	static inline const float kBlockWidth_ = 48.0f;
	//高さ
	static inline const float kBlockHeight_ = 48.0f;

	bool isBlockAndPlayerBottomCollision_ = false;
	bool isPreBlockAndPlayerBottomCollision = false;

	//座標関係
	Matrix3x3 worldMatrix_;
	Matrix3x3 wvpVpMatrix_;

	//鍵が描画されたか
	bool isKeyDraw_ = false;

	//スクリーン座標
	Vector2 screenPosition_;

	//ブロック
	Block blockTop_[10];
	Block blockBottom_[10];

	//落とし穴
	AABB fallAABB_[10];

	//ブロック落とす用のタイマー
	float blockFallTimer = 0.0f;
	//落下するときの初期位置
	Vector2 startBlockPosition;
	//落下するときの最終位置
	Vector2 endBlockPosition;

	//背景
	float backTreePos[8] = { -2160 ,-1440,-720, 0,720,1440,2160 };
	float frontTreePos[8] = { -2160 ,-1440,-720,0,720,1440,2160 };
	int backLocalPos[8] = {};
	int frontLocalPos[8] = {};

	//スクロール
	float frontTreeScroll_ = 0;
	float backTreeScroll_ = 0;

	//背景の幅
	int kWidth_ = 720;
	int kHeight_ = 360;
	int kMostPos_ = 1280;
	Vector2 kBackPos_ = { 640.0f , 355.0f };

	//背景画像
	//上の背景
	//int bg_groundTexture = Novice::LoadTexture("./Resources./background./bg_ground.png");
	//前の木
	int frontTreeTexture = Novice::LoadTexture("./Resources./background./frontTree.png");
	//奥の木
	int backTreeTexture = Novice::LoadTexture("./Resources./background./farTree.png");
	//前の草
	int frontGrassTexture = Novice::LoadTexture("./Resources./background./frontGrass.png");
	//真ん中の草
	int centerGrassTexture = Novice::LoadTexture("./Resources./background./centerGrass.png");
	//奥の草
	int farGrassTexture = Novice::LoadTexture("./Resources./background./farGrass.png");

	//下の背景
	int bg_underTexture = Novice::LoadTexture("./Resources./background./underBack.png");

	//マップチップの画像
	//上のブロックテクスチャ
	int blockTexture = Novice::LoadTexture("./Resources./mapChip./block.png");
	//下のブロックテクスチャ
	int stoneTexture = Novice::LoadTexture("./Resources./mapChip./stone.png");
	//鍵テクスチャ
	int keyTexture = Novice::LoadTexture("./Resources./mapChip./key.png");
	//扉(ゴール)テクスチャ
	int goalCloseTexture = Novice::LoadTexture("./Resources./mapChip./door_close.png");
	int goalOpenTexture = Novice::LoadTexture("./Resources./mapChip./door_open.png");
	//アニメーションタイマー
	int animationTimer = 0;
	int backAnimationTimer = 0;
	//アニメーションタイマーを0に戻す
	int animationTimerReset = 60;
	int backAnimationTimerReset = 28;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


};

