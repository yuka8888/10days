#pragma once
#include "mt.h"
#include "Novice.h"

class PlayerBottom
{
public:
	PlayerBottom();

	~PlayerBottom();

	void Initialize();

	void Update();

	void Draw(Camera camera);

	void Move();

	void PushBlock();

	AABB GetAABB();

	//当たった場合の処理
	void OnCollision();

	Vector2 GetVelocity();

private:
	//幅
	float kWidth_ = 32.0f;
	//高さ
	float kHeight_ = 64.0f;

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

	//AABB
	AABB aabb_;

	bool isPushBlock_ = false;
};

