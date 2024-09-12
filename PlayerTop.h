#pragma once
#define NOMINMAX
#include "mt.h"
#include "Novice.h"
#include "algorithm"
#include "imgui.h"

class MapChipManager;

class PlayerTop
{
public:
	PlayerTop();

	~PlayerTop();

	void Initialize();

	/// <summary>
	/// 下のキャラクターが動くときの更新処理
	/// </summary>
	void PlayerStopPhaseUpdate();

	/// <summary>
	/// 上のキャラクターが動くときの更新処理
	/// </summary>
	void PlayerMovePhaseUpdate();
	
	/// <summary>
	/// 最終的に移動させる
	/// </summary>
	void MoveResult();

	void Draw();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	AABB GetAABB();

	//当たった場合の処理
	void OnCollision();

	/// <summary>
	/// マップチップとの当たり判定上
	/// </summary>
	void MapCollisionTop();

	/// <summary>
	/// マップチップとの当たり判定下
	/// </summary>
	void MapCollisionBottom();

	/// <summary>
	/// マップチップの当たり判定右
	/// </summary>
	void MapCollisionRight();

	/// <summary>
	/// マップチップとの当たり判定左
	/// </summary>
	void MapCollisionLeft();

	void SetMapChipField(MapChipManager* mapChipManager);

	//アニメーションタイマーを状態によって切り替える
	void AnimationTimerChange();

	/// <summary>
	/// 上下の場所入れ替え
	/// </summary>
	void SwapTopBottom();
	
	void ScrollMove();

	void SetBlockTop(Block* block);

	void SetBlockBottom(Block* block);

	void SetTopBlockNum(uint32_t blockNum);

	void SetBottomBlockNum(uint32_t blockNum);

	bool HaveKey();

	void HaveKey(bool haveKey);

	bool IsPressSwitch();

	void IsPressSwitch(bool isPressSwitch_);

	void IsKeyDraw(bool isKeyDraw);

	bool IsMagicCircleTouch();

	void SetCamera(Camera camera);

	Camera GetCamera();

	bool IsGoal();

	float GetFrontTreeScroll();

	float GetBackTreeScroll();
	
	void SetFrontTreeScroll(float frontTreeScroll);

	void SetBackTreeScroll(float backTreeScroll);

	void SetIsChanged(bool isChanged);
private:
	//向いている方向
	Direction direction = Direction::kRightStand;

	float kWindowHeight = 720.0f;
	float kWindowWidth = 1280.0f;

	//幅
	float kWidth_ = 32.0f;
	//高さ
	float kHeight_ = 64.0f;
	float kChangeHeight = kHeight_ + 32;
	//アニメーションタイマー
	int animationTimer = 0;
	//アニメーションタイマーを0に戻す
	int animationTimerReset = 0;
	//操作キャラ変更アニメーションタイマー
	int changeAnimationTimer = 0;
	//操作キャラ変更アニメーションタイマーのリセット
	int changeAnimationTimerReset = 120;

	//プレイヤーの画像
	//歩き　右
	int walkRight = Novice::LoadTexture("./Resources./player_girl./player_walkRight_girl.png");
	//歩き　左
	int walkLeft = Novice::LoadTexture("./Resources./player_girl./player_walkLeft_girl.png");

	//待機　右
	int standingRight = Novice::LoadTexture("./Resources./player_girl./player_StandingRight_girl.png");
	//待機　左
	int standingLeft = Novice::LoadTexture("./Resources./player_girl./player_StandingLeft_girl.png");
	//ジャンプ　右
	int jumpRight = Novice::LoadTexture("./Resources./player_girl./player_JumpRight_girl.png");
	//ジャンプ　左
	int jumpLeft = Novice::LoadTexture("./Resources./player_girl./player_JumpLeft_girl.png");
	
	//操作キャラ変更後
	//歩き　右
	int walkChangeRight = Novice::LoadTexture("./Resources./player_girl./player_walkChangeRight_girl.png");
	//歩き　左
	int walkChangeLeft = Novice::LoadTexture("./Resources./player_girl./player_walkChangeLeft_girl.png");
	//待機　右
	int standingChangeRight = Novice::LoadTexture("./Resources./player_girl./player_StandingChangeRight_girl.png");
	//待機　左
	int standingChangeLeft = Novice::LoadTexture("./Resources./player_girl./player_StandingChangeLeft_girl.png");

	bool isChanged_ = false;
	//ゴールしたか
	bool isGoal_ = false;

	//鍵を持っているか
	bool haveKey_ = false;

	//魔法陣に触れているか
	bool isMagicCircleTouch_ = false;

	//鍵が描画されたか
	bool isKeyDraw_ = false;

	float kBottomGround_ = 56.0f;

	float kTopGround_ = 48.0f * 8.0f + 24.0f + kHeight_ / 2.0f;

	float nowGround_ = kTopGround_;

	//初期位置
	Vector2 initialPosition_ = {0.0f,0.0f };
	//座標
	Vector2 translation_ = {100.0f, nowGround_ };

	//1ループ前の座標
	Vector2 preTranslation_ = { 0.0f, kTopGround_ + 10.0f };
	//拡縮
	Vector2 scale_ = {1.0f, 1.0f};
	//回転
	float rotate_ = 0.0f;
	//移動量
	Vector2 velocity_ = { 0.0f,0.0f };
	//加速量
	Vector2 kAcceleration_ = { 0.0f, -0.5f };

	//最高落下速度
	float kMaxFallSpeed = -10.0f;

	//地面についているか
	bool isGround_ = true;

	//ジャンプしているか
	bool isJump = false;

	//ブロックの上にいるか
	bool isLanding_ = false;

	//天井に当たっているか
	bool isCollideCeiling_ = false;

	//スイッチが押されたか
	bool isPressSwitch_ = false;

	//スクロール
	float frontTreeScroll_ = 0.0f;
	float oldFrontTreeScroll_ = 0.0f;
	float backTreeScroll_ = 0.0f;
	float oldBackTreeScroll_ = 0.0f;

	Block blockTop_[10];
	Block blockBottom_[10];

	uint32_t blockTopNum_;
	uint32_t blockBottomNum_;
	
	MapChipType collisionBlockType_ = MapChipType::kBlank;

	//したがぶロックについているか
	bool isBottomHit_ = false;

	//壁に当たったか
	bool isContactWall_ = false; 

	//座標関係
	Matrix3x3 worldMatrix_;
	Matrix3x3 wvpVpMatrix_;
	
	//カメラ関係
	Camera camera_;
	//スクリーン座標
	Vector2 screenPosition_;

	//マップチップ
	MapChipManager* mapChipManager_ = nullptr;

	//AABB
	AABB aabb_;

	Vector2 CornerPosition(const Vector2& center, Corner corner);
};

