#pragma once
#include <memory>
#include "Iscene.h"
#include "titleScene.h"
#include "PlayScene.h"
#include "ClearScene.h"

class SceneManager
{
private:
	//シーンを保持するメンバ変数
	IScene* sceneArr_[3];

	//度のシーンを呼び出すかを管理する変数
	int currentSceneNo_; // 現在のシーン
	int prevSceneNo_; //前のシーン

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

public:
	SceneManager();
	~SceneManager();


	int Run(); //この関数でゲームループを呼び出す
};

