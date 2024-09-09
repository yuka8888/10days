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
	void PlayerBottomPhaseUpdate();

	/// <summary>
	/// 上のキャラクターが動くときの更新処理
	/// </summary>
	void PlayerTopPhaseUpdate();
	
	/// <summary>
	/// 最終的に移動させる
	/// </summary>
	void MoveResult();

	void Draw(Camera camera);

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

private:
	//向いている方向
	Direction direction = Direction::kRightStand;

	//幅
	float kWidth_ = 32.0f;
	//高さ
	float kHeight_ = 64.0f;

	//地面座標
	float kGround_ = 48.0f * 8.0f + 24.0f + kHeight_ / 2.0f;

	//初期位置
	Vector2 initialPosition_ = {0.0f,0.0f };
	//座標
	Vector2 translation_ = {50.0f, kGround_};

	//1ループ前の座標
	Vector2 preTranslation_ = { 0.0f, kGround_ + 10.0f };
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

	//ブロックの上にいるか
	bool isLanding_ = false;

	//天井に当たっているか
	bool isCollideCeiling_ = false;
	
	MapChipType collisionBlockType_ = MapChipType::kBlank;

	//したがぶロックについているか
	bool isBottomHit_ = false;

	//壁に当たったか
	bool isContactWall_ = false; 

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

};

