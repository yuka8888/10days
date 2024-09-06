#pragma once
#include "IScene.h"
#include "Novice.h"

class TitleScene :public IScene
{
public:
	TitleScene();


	void Initialize() override;
	void PlayerBottomMoveUpdate() override;
	void Draw() override;

private:

};


