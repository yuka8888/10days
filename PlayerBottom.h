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

private:
	Vector2 initialPosition_ = { 0.0f, 100.0f };
	Vector2 translation_ = { 0.0f, 0.0f };
	Vector2 scale_ = { 1.0f, 1.0f };
	float rotate_ = 0.0f;
	Vector2 velocity_ = { 0.0f,0.0f };

	//座標関係
	Matrix3x3 worldMatrix_;
	Matrix3x3 wvpVpMatrix_;

	//スクリーン座標
	Vector2 screenPosition_;

};

