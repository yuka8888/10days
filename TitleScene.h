#pragma once
#include "IScene.h"
#include "Novice.h"

class TitleScene :public IScene
{
public:
	TitleScene();


	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

};


