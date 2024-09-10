#pragma once
#include "mt.h"
class CameraManager
{
public:
	void Initialize();

	/// <summary>
	/// viewPortPositionの変更
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	void SetViewPortPosition(Vector2 translation);

	Camera GetCamera();

private:
	Camera camera_;

};

