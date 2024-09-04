#pragma once
#include "IScene.h"
#include "Novice.h"

class ClearScene :public IScene
{
public:
	ClearScene();


	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

};
