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

	void Sound();
private:
	//ゲームのフェーズ
	enum class Phase {
		kMovePlayerTop, //女の子が動く
		kMovePlayerBottom, //男の子が動く
		kMoveAll, //どっちも動く
	};

	//現在のステージ
	uint32_t StageNo = 1;

	//ステージをクリアしたか
	bool isStageClear = false;

	//現在のフェーズ
	Phase phase = Phase::kMovePlayerTop;

	float kWindowHeight = 720.0f;
	float kWindowWidth = 1280.0f;

	//プレイヤー
	PlayerTop* playerTop_ = nullptr;
	PlayerBottom* playerBottom_ = nullptr;

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

	//スクリーン座標
	Vector2 screenPosition_;

	//ブロック
	Block block[10];

	//落とし穴
	AABB fallAABB_[10];

	//ブロック落とす用のタイマー
	float blockFallTimer = 0.0f;
	//落下するときの初期位置
	Vector2 startBlockPosition;
	//落下するときの最終位置
	Vector2 endBlockPosition;

	//背景画像
	//上の背景
	int bg_groundTexture = Novice::LoadTexture("./Resources./background./bg_ground.png");
	//下の背景
	int bg_underTexture = Novice::LoadTexture("./Resources./background./bg_under.png");
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
	//アニメーションタイマーを0に戻す
	int animationTimerReset = 60;

	//音
	//BGM
	///鍵取得SE読み込み
	int gameSceneBGM = Novice::LoadAudio("./Resources./sound././BGM./gameSceneBGM.wav");
	///プレイハンドルの宣言
	int playgameScene = -1;
	//SE

	//操作キャラ変更SE
	int characterChangeSE = Novice::LoadAudio("./Resources./sound././SE./characterChangeSE.wav");
	///プレイハンドルの宣言
	int playcharacterChange = -1;
	///鍵取得SE読み込み
	int getKeySE = Novice::LoadAudio("./Resources./sound././SE./getKeySE.wav");
	///プレイハンドルの宣言
	int playGetKey = -1;
	//鍵の前取得状態
	bool preHaveKey = 0;
	///ブロック押してるSE読み込み
	int pushBlockSE = Novice::LoadAudio("./Resources./sound././SE./pushBlockSE.wav");
	///プレイハンドルの宣言
	int playPushBlock = -1;
	///ブロック押ちたSE読み込み
	int blockFitSE = Novice::LoadAudio("./Resources./sound././SE./blockFitSE.wav");
	///プレイハンドルの宣言
	int playPlockFit = -1;
	//ブロックの前取得状態
	bool preHaveBlock = 0;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	
};

