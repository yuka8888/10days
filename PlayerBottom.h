#pragma once
#include "mt.h"
#include "Novice.h"
#include "imgui.h"

class PlayerBottom
{
public:
	PlayerBottom();

	~PlayerBottom();

	void Initialize();

	/// <summary>
	/// 下のキャラクターが動くときの更新処理
	/// </summary>
	void PlayerBottomPhaseUpdate();

	/// <summary>
	/// 上のキャラクターが動くときの更新処理
	/// </summary>
	void PlayerTopPhaseUpdate();

	void Draw(Camera camera);

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 最終的に移動させる
	/// </summary>
	void MoveResult();

	void PushTwoBlocks(Block block);

	AABB GetAABB();

	//当たった場合の処理
	void OnCollision();

	Vector2 GetVelocity();

	Vector2 GetTranslation();

	Direction GetDirection();

	void isPushTwoBlocks(bool isPushTwoBlocks);

	Vector2 GetSize();

	void SetTranslation(Vector2 translation);

	//アニメーションタイマーを状態によって切り替える
	void AnimationTimerChange();

private:
	//向いている方向
	Direction direction = Direction::kRightStand;
	//幅
	float kWidth_ = 32.0f;
	float pushWidth = kWidth_ + 16;
	//高さ
	float kHeight_ = 64.0f;

	//アニメーションタイマー
	int animationTimer = 0;
	//アニメーションタイマーを0に戻す
	int animationTimerReset = 0;
	//プレイヤーの画像
	//歩き　右
	int walkRight = Novice::LoadTexture("./Resources./player_boy./player_walkRight_boy.png");
	//歩き　左
	int walkLeft = Novice::LoadTexture("./Resources./player_boy./player_walkLeft_boy.png");

	//待機　右
	int standingRight = Novice::LoadTexture("./Resources./player_boy./player_StandingRight_boy.png");
	//待機　左
	int standingLeft = Novice::LoadTexture("./Resources./player_boy./player_StandingLeft_boy.png");
	//ブロック押す
	int pushBlock = Novice::LoadTexture("./Resources./player_boy./player_PushRight_boy.png");


	Vector2 initialPosition_ = { 0.0f, 0.0f };
	Vector2 translation_ = { 0.0f, 56.0f };
	Vector2 scale_ = { 1.0f, 1.0f };
	float rotate_ = 0.0f;
	Vector2 velocity_ = { 0.0f,0.0f };

	//座標関係
	Matrix3x3 worldMatrix_;
	Matrix3x3 wvpVpMatrix_;

	//スクリーン座標
	Vector2 screenPosition_;

	//ブロックを二個押す
	bool isPushTwoBlocks_ = false;

	//AABB
	AABB aabb_;

	int isPushBlock_ = false;

};

