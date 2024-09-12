#pragma once
#include"Novice.h"
#include"Vector2.h"
#include"mt.h"

class TitleText
{
public:
	void Initialize();
	void Update();
	void Draw();
private:
	//絵
	int titleText = Novice::LoadTexture("./Resources./Title./title.png");
	int pressSpaceText = Novice::LoadTexture("./Resources./Title./pressSPACE.png");
	//動く
	float moveTimer_ = 0.0f;
	//場所
	Vector2 titleTextPos_{};
	Vector2 pressSpaceTextPos_{};
};
