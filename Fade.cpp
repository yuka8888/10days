#include "Fade.h"

void Fade::Initialize() {
}

void Fade::Update() {
	switch (status_) {
		case Status::None:
			// 何もしない
			break;
		case Status::FadeIn:
			// 1フレーム文の秒数をカウントアップ
			counter_ += 1.0f / 60.0f;

			// フェード継続時間に達したら打ち止め
			if (counter_ >= duration_) {
				counter_ = duration_;
			}
			// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ地を大きくする
			transparency = int(std::clamp((duration_ - counter_) / duration_, 0.0f, 1.0f) * 256.0f);

			color = transparency;
			break;
		case Status::FadeOut:
			// 1フレーム文の秒数をカウントアップ
			counter_ += 1.0f / 60.0f;

			// フェード継続時間に達したら打ち止め
			if (counter_ >= duration_) {
				counter_ = duration_;
			}
			// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ地を大きくする
			transparency = int(std::clamp(counter_ / duration_, 0.0f, 1.0f) * 255.0f);

			color = transparency;

			break;
	}
}

void Fade::Draw() {
	if (status_ == Status::None) {
		return;
	}
	Novice::DrawSprite(0, 0, fadeTexture, 1.0f, 1.0f, 0.0f, color);
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
	if (status_ == Status::FadeIn) {
		color = 0x000000FF;
	}
	else if (status_ == Status::FadeOut) {
		color = 0x00000000;
	}
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const {
	switch (status_) {
		case Fade::Status::FadeIn:
		case Fade::Status::FadeOut:
			if (counter_ >= duration_) {
				return true;
			}
			else {
				return false;
			}
	}

	return true;
}

void Fade::SetSize(const Vector2& size)
{
	size_ = size;
}

Fade::Status Fade::GetStatus() { return status_; }
