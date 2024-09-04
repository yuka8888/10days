#pragma once
#include "IScene.h"
#include "Novice.h"

class PlayScene :public IScene
{
public:
	PlayScene();


	void Initialize() override;
	void Update() override;
	void Draw() override;

private:

};

