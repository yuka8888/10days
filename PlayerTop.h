#pragma once
#include "mt.h"
#include "Novice.h"

class PlayerTop
{
public:
	PlayerTop();

	~PlayerTop();

	void Initialize();

	void Update();

	void Draw(Camera camera);

	void Move();

private:
	//地面座標
	float kGround = 360.0f;

	//初期位置
	Vector2 initialPosition_ = {0.0f,kGround };
	//座標
	Vector2 translation_ = {0.0f, 0.0f};
	//拡縮
	Vector2 scale_ = {1.0f, 1.0f};
	//回転
	float rotate_ = 0.0f;
	//移動量
	Vector2 velocity_ = { 0.0f,0.0f };
	//加速量
	Vector2 kAcceleration_ = { 0.0f, -0.5f };

	//地面についているか
	bool isGround = true;

	//ジャンプしているか
	bool isJump = false;

	//座標関係
	Matrix3x3 worldMatrix_;
	Matrix3x3 wvpVpMatrix_;

	//スクリーン座標
	Vector2 screenPosition_;
};

