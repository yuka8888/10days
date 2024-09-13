
#include "ClearText.h"

void ClearText::Initialize()
{
	gameClearTextPos_ = { 200.0f,240.0f };
	pressSpaceTextPos_ = { 370.0f,500.0f };
}

void ClearText::Update()
{
	moveTimer_ += 2.0f / 30.0f;
	gameClearTextPos_.y += std::sinf(moveTimer_);
}

void ClearText::Draw()
{
	Novice::DrawSprite(int(gameClearTextPos_.x), int(gameClearTextPos_.y), clearText, 1.0f, 1.0f, 0.0f, WHITE);
	Novice::DrawSprite(int(pressSpaceTextPos_.x), int(pressSpaceTextPos_.y), pressSpaceText, 1.0f, 1.0f, 0.0f, WHITE);
}
