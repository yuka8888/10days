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
		kMovePlayerTop, //女の子が動く
		kMovePlayerBottom, //男の子が動く
		kMoveAll, //どっちも動く
	};

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


	//座標関係
	Matrix3x3 worldMatrix_;
	Matrix3x3 wvpVpMatrix_;

	//スクリーン座標
	Vector2 screenPosition_;

	//ブロック
	Block block[10];

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	
};

