#pragma once
enum Scene {
	kTitle,
	kPlay,
	kClear
};
class IScene
{
protected:
	//シーン番号を管理する変数
	static int sceneNo;

public:
	//継承先で実装される関数
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~IScene();

	int GetSceneNo();
};

