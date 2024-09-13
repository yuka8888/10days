
#include "TitleScene.h"

TitleScene::TitleScene()
{
	delete titleText;
	delete fade_;
}

void TitleScene::Initialize()
{
	titleText = new TitleText;
	titleText->Initialize();

	fade_ = new Fade();
	fade_->Initialize();
}

void TitleScene::Update()
{
	fade_->Update();
	titleText->Update();

	//背景
	underAnimationTimer_++;
	if (underAnimationTimer_ >= underAnimationTimerReset_)
	{
		underAnimationTimer_ = 0;
	}
	//スクロール
	backTreeScroll_ += 0.08f;
	frontTreeScroll_ += 0.2f;
	for (int i = 0; i < 7; ++i)
	{
		backLocalPos[i] = int(backTreePos[i] + backTreeScroll_);
	}
	for (int i = 0; i < 7; ++i)
	{
		frontLocalPos[i] = int(frontTreePos[i] + frontTreeScroll_);
	}
	if (Novice::CheckHitKey(DIK_SPACE) && !(fade_->GetStatus() == Fade::Status::FadeOut)) {
		fade_->Start(Fade::Status::FadeOut, 1.0f);
	}

	if ((fade_->GetStatus() == Fade::Status::FadeOut) && (fade_->IsFinished() == true)) {
		sceneNo = kPlay;
	}
}

void TitleScene::Draw()
{
	//背景の描画
	//上
	//奥の草
	Novice::DrawSprite(0, 0, farGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	Novice::DrawSprite(kWidth_, 0, farGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	//後ろの木
	for (int i = 0; i < 7; ++i)
	{
		Novice::DrawSprite(backLocalPos[i], 0, backTreeTexture, 1.0f, 1.0f, 0.0f, WHITE);
	}
	//真ん中の草
	Novice::DrawSprite(0, 0, centerGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	Novice::DrawSprite(kWidth_, 0, centerGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	//前の木
	for (int i = 0; i < 7; ++i)
	{
		Novice::DrawSprite(frontLocalPos[i], 0, frontTreeTexture, 1.0f, 1.0f, 0.0f, WHITE);
	}
	//前の草
	Novice::DrawSprite(0, 0, frontGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);
	Novice::DrawSprite(kWidth_, 0, frontGrassTexture, 1.0f, 1.0f, 0.0f, WHITE);

	//下
	Novice::DrawQuad(0, int(kBackPos_.y), int(kBackPos_.x), int(kBackPos_.y), 0, kWidth_, int(kBackPos_.x), kWidth_, kWidth_ * (underAnimationTimer_ / 18), 0, kWidth_, kHeight_, bg_underTexture, WHITE);
	Novice::DrawQuad(int(kBackPos_.x), int(kBackPos_.y), kMostPos_, int(kBackPos_.y), int(kBackPos_.x), kWidth_, kMostPos_, kWidth_, kWidth_ * (underAnimationTimer_ / 18), 0, kWidth_, kHeight_, bg_underTexture, WHITE);

	titleText->Draw();
	fade_->Draw();
}
