#pragma once
#include "IScene.h"
#include "Novice.h"
#include "CameraManager.h"
#include "PlayerTop.h"
#include "PlayerBottom.h"

class PlayScene :public IScene
{
public:
	PlayScene();

	~PlayScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	//プレイヤー
	PlayerTop* playerTop_;
	PlayerBottom* playerBottom_;

	//カメラの管理
	CameraManager* cameraManager_;
};

