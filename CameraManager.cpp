#include "CameraManager.h"

void CameraManager::Initialize()
{
	//カメラ
	camera_.worldMatrix.m[0][0] = 1;
	camera_.worldMatrix.m[0][1] = 0;
	camera_.worldMatrix.m[0][2] = 0;
	camera_.worldMatrix.m[1][0] = 0;
	camera_.worldMatrix.m[1][1] = 1;
	camera_.worldMatrix.m[1][2] = 0;
	camera_.worldMatrix.m[2][0] = 640;//
	camera_.worldMatrix.m[2][1] = 360;//
	camera_.worldMatrix.m[2][2] = 1;

	camera_.vertex.left = -640;
	camera_.vertex.top = 360;
	camera_.vertex.right = 640;
	camera_.vertex.bottom = -360;

	camera_.viewPortPosition = {};
	camera_.viewPortSize = { 1280.0f, 720.0f };

}

void CameraManager::AddViewPortPosition(Vector2 velocity)
{
	camera_.viewPortPosition = camera_.viewPortPosition + velocity;
}

Camera CameraManager::GetCamera()
{
	return camera_;
}
