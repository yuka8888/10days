
#pragma once
#include"Novice.h"
#include"Vector2.h"
#include <cmath>
#include <iostream>
#include"mt.h"

class ClearText
{
public:
	void Initialize();
	void Update();
	void Draw();
private:
	//絵
	int clearText = Novice::LoadTexture("./Resources./Title./GameClear.png");
	int pressSpaceText = Novice::LoadTexture("./Resources./Title./pressSPACE.png");
	//動く
	float moveTimer_ = 0.0f;
	//場所
	Vector2 gameClearTextPos_{};
	Vector2 pressSpaceTextPos_{};
};
