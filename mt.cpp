#include "mt.h"

Vector2 operator+(Vector2 num1, Vector2 num2) {
	num1.x += num2.x;
	num1.y += num2.y;

	return num1;
}

Vector2 operator-(Vector2 num1, Vector2 num2) {
	num1.x -= num2.x;
	num1.y -= num2.y;

	return num1;
}

/// <summary>
/// 行列の積
/// </summary>
/// <param name="matrix1"></param>
/// <param name="matrix2"></param>
/// <returns></returns>
Matrix3x3 Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2) {
	Matrix3x3 matrix3 = {};

	matrix3.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] + matrix1.m[0][2] * matrix2.m[2][0];
	matrix3.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] + matrix1.m[0][2] * matrix2.m[2][1];
	matrix3.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] + matrix1.m[0][2] * matrix2.m[2][2];
	matrix3.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] + matrix1.m[1][2] * matrix2.m[2][0];
	matrix3.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] + matrix1.m[1][2] * matrix2.m[2][1];
	matrix3.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] + matrix1.m[1][2] * matrix2.m[2][2];
	matrix3.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] + matrix1.m[2][2] * matrix2.m[2][0];
	matrix3.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] + matrix1.m[2][2] * matrix2.m[2][1];
	matrix3.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] + matrix1.m[2][2] * matrix2.m[2][2];
	return matrix3;
}

/// <summary>
/// 逆行列
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
Matrix3x3 Inverse(Matrix3x3 matrix) {
	Matrix3x3 matrix2 = {};
	float num = 1.0f / (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][1] +
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]);

	matrix2.m[0][0] = num * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]);
	matrix2.m[0][1] = num * (-(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]));
	matrix2.m[0][2] = num * (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]);
	matrix2.m[1][0] = num * (-(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]));
	matrix2.m[1][1] = num * (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]);
	matrix2.m[1][2] = num * (-(matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]));
	matrix2.m[2][0] = num * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]);
	matrix2.m[2][1] = num * (-(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]));
	matrix2.m[2][2] = num * (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]);

	return matrix2;
}

/// <summary>
/// 2次元ベクトルを同次座標に変換
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector2 Transform(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result = {};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0);

	result.x /= w;
	result.y /= w;

	return result;
}

/// <summary>
/// 回転行列
/// </summary>
/// <param name="theta"></param>
/// <returns></returns>
Matrix3x3 MakeRotateMatrix3x3(float theta) {
	Matrix3x3 matrix = {};
	matrix.m[0][0] = cosf(theta);
	matrix.m[0][1] = sinf(theta);
	matrix.m[1][0] = -1.0f * sinf(theta);
	matrix.m[1][1] = cosf(theta);
	matrix.m[2][2] = 1;
	return matrix;
}

/// <summary>
/// 平行移動行列の作成
/// </summary>
/// <param name="translate"></param>
/// <returns></returns>
Matrix3x3 MakeTransLateMatrix(Vector2 translate) {
	Matrix3x3 matrix = {};
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[2][0] = translate.x;
	matrix.m[2][1] = translate.y;

	return matrix;
}

/// <summary>
/// 拡大縮小行列の作成
/// </summary>
/// <param name="scale"></param>
/// <returns></returns>
Matrix3x3 MakeScaleMatrix(Vector2 scale) {
	Matrix3x3 matrix = {};
	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = 1;
	return matrix;
}

/// <summary>
/// アフィン行列
/// </summary>
/// <param name="scale"></param>
/// <param name="rotate"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix3x3 MakeAffineMatrix(Vector2 scale, float rotate, Vector2 translate) {
	return Multiply(Multiply(MakeScaleMatrix(scale), MakeRotateMatrix3x3(rotate)), MakeTransLateMatrix(translate));
}

/// <summary>
/// 正射影行列
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <returns></returns>
Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom) {
	Matrix3x3 matrix = {};

	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[2][0] = (left + right) / (left - right);
	matrix.m[2][1] = (top + bottom) / (bottom - top);
	matrix.m[2][2] = 1.0f;

	return matrix;
}

/// <summary>
/// ビューポート行列
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
Matrix3x3 MakeViewportMatrix(float left, float top, float width, float height) {
	Matrix3x3 matrix = {};

	matrix.m[0][0] = width / 2.0f;
	matrix.m[1][1] = -height / 2.0f;
	matrix.m[2][0] = left + width / 2.0f;
	matrix.m[2][1] = top + height / 2.0f;
	matrix.m[2][2] = 1.0f;

	return matrix;
}

/// <summary>
/// wvpVp変換
/// </summary>
/// <param name="worldMatrix"></param>
/// <param name="cameraWorldMatrix"></param>
/// <param name="cameraVertex"></param>
/// <param name="viewPortPosition"></param>
/// <param name="viewPortSize"></param>
/// <returns></returns>
Matrix3x3 MakewvpVpMatrix(Matrix3x3 worldMatrix, Matrix3x3 cameraWorldMatrix, Vertex cameraVertex, Vector2 viewPortPosition, Vector2 viewPortSize) {
	Matrix3x3 viewMatrix = Inverse(cameraWorldMatrix);
	Matrix3x3 orthoMatrix = MakeOrthographicMatrix(cameraVertex.left, cameraVertex.top, cameraVertex.right, cameraVertex.bottom);
	Matrix3x3 viewportMatrix = MakeViewportMatrix(viewPortPosition.x, viewPortPosition.y, viewPortSize.x, viewPortSize.y);

	Matrix3x3 vpVpMatrix = Multiply(viewMatrix, orthoMatrix);
	vpVpMatrix = Multiply(vpVpMatrix, viewportMatrix);

	return Multiply(worldMatrix, vpVpMatrix);

}

/// <summary>
/// vpVp変換
/// </summary>
/// <param name="cameraWorldMatrix"></param>
/// <param name="cameraVertex"></param>
/// <param name="viewPortPosition"></param>
/// <param name="viewPortSize"></param>
/// <returns></returns>
Matrix3x3 MakevpVpMatrix(Matrix3x3 worldMatrix, Vertex vertex, Vector2 viewPortPosition, Vector2 viewPortSize) {
	Matrix3x3 viewMatrix = Inverse(worldMatrix);
	Matrix3x3 orthoMatrix = MakeOrthographicMatrix(vertex.left, vertex.top, vertex.right, vertex.bottom);
	Matrix3x3 viewportMatrix = MakeViewportMatrix(viewPortPosition.x, viewPortPosition.y, viewPortSize.x, viewPortSize.y);

	Matrix3x3 vpVpMatrix = Multiply(viewMatrix, orthoMatrix);
	return Multiply(vpVpMatrix, viewportMatrix);
}

/// <summary>
/// AABBの当たり判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool isCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y)) {
		return true;
	}

	return false;
}
