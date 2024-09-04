#include "IScene.h"
//タイトルシーンで初期化
int IScene::sceneNo = kTitle;

//仮想デストラクタの定義

IScene::~IScene() {};

int IScene::GetSceneNo() { return sceneNo; }