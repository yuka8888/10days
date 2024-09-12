#pragma once
#include "IScene.h"
#include"TitleText.h"

class TitleScene :public IScene
{
public:

	enum class Phase
	{
		kFadeIn,
		kMain,
		kFadeOut,
	};

	TitleScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

	TitleText* titleText = nullptr;

	//前の木
	int frontTreeTexture = Novice::LoadTexture("./Resources./background./frontTree.png");
	//奥の木
	int backTreeTexture = Novice::LoadTexture("./Resources./background./farTree.png");
	//前の草
	int frontGrassTexture = Novice::LoadTexture("./Resources./background./frontGrass.png");
	//真ん中の草
	int centerGrassTexture = Novice::LoadTexture("./Resources./background./centerGrass.png");
	//奥の草
	int farGrassTexture = Novice::LoadTexture("./Resources./background./farGrass.png");

	//下の背景
	int bg_underTexture = Novice::LoadTexture("./Resources./background./underBack.png");

	//背景
	float backTreePos[7] = { -3600,-2880,-2160,-1440,-720,0,720 };
	float frontTreePos[7] = { -3600,-2880,-2160,-1440,-720,0,720 };
	int backLocalPos[7] = {};
	int frontLocalPos[7] = {};

	//背景の幅
	int kWidth_ = 720;
	int kHeight_ = 360;
	int kMostPos_ = 1280;
	Vector2 kBackPos_ = { 640.0f , 355.0f };

	//スクロール
	float frontTreeScroll_ = 0.0f;
	float backTreeScroll_ = 0.0f;

	//アニメーションタイマー
	int underAnimationTimer_ = 0;
	//アニメーションタイマーを0に戻す
	int underAnimationTimerReset_ = 72;
};


