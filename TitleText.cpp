#include "TitleText.h"

void TitleText::Initialize()
{
	titleTextPos_ = { 200.0f,240.0f };
	pressSpaceTextPos_ = { 370.0f,500.0f };
}

void TitleText::Update()
{
	moveTimer_ += 2.0f / 30.0f;
	titleTextPos_.y += std::sinf(moveTimer_);
}

void TitleText::Draw()
{
	Novice::DrawSprite(int(titleTextPos_.x), int(titleTextPos_.y), titleText, 1.0f, 1.0f, 0.0f, WHITE);
	Novice::DrawSprite(int(pressSpaceTextPos_.x), int(pressSpaceTextPos_.y), pressSpaceText, 1.0f, 1.0f, 0.0f, WHITE);
}
