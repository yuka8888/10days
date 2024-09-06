#pragma once
#include "IScene.h"
#include "Novice.h"

class ClearScene :public IScene
{
public:
	ClearScene();


	void Initialize() override;
	void PlayerBottomMoveUpdate() override;
	void Draw() override;

private:

};
