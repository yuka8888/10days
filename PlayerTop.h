#pragma once
#define NOMINMAX
#include "mt.h"
#include "Novice.h"
#include "algorithm"

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
	void PlayerBottomPhaseUpdate();

	/// <summary>
	/// 上のキャラクターが動くときの更新処理
	/// </summary>
	void PlayerTopPhaseUpdate();
	
	/// <summary>
	/// 最終的に移動させる
	/// </summary>
	void MoveResult();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void MapCollision();

	void Draw(Camera camera);

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	AABB GetAABB();

	//当たった場合の処理
	void OnCollision();

	void SetMapChipField(MapChipManager* mapChipManager);

private:
	//向いている方向
	Direction direction = Direction::kRightStand;

	
	//幅
	float kWidth_ = 32.0f;
	//高さ
	float kHeight_ = 64.0f;

	//地面座標
	float kGround_ = 48.0f * 9.0f;

	//初期位置
	Vector2 initialPosition_ = {0.0f,0.0f };
	//座標
	Vector2 translation_ = {0.0f, kGround_};
	//拡縮
	Vector2 scale_ = {1.0f, 1.0f};
	//回転
	float rotate_ = 0.0f;
	//移動量
	Vector2 velocity_ = { 0.0f,0.0f };
	//加速量
	Vector2 kAcceleration_ = { 0.0f, -0.5f };

	//地面についているか
	bool isGround_ = true;

	//ジャンプしているか
	bool isJump = false;

	bool isLanding_ = false;

	bool isCollideCeiling_ = false;

	//したが無ロックについているか
	bool isBottomHit_ = false;

	//座標関係
	Matrix3x3 worldMatrix_;
	Matrix3x3 wvpVpMatrix_;

	//スクリーン座標
	Vector2 screenPosition_;

	//マップチップ
	MapChipManager* mapChipManager_ = nullptr;

	//AABB
	AABB aabb_;
	Vector2 CornerPosition(const Vector2& center, Corner corner);

	void MapCollisionTop();
	void MapCollisionBottom();
	void MapCollisionLeft();
	void MapCollisionRight();

	void SwitchToOnGround();

};

