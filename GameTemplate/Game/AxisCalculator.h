#pragma once
class AxisCalculator
{
private:
	/// <summary>
	/// 共分散行列から軸を計算する。
	/// </summary>
	/// <param name="newAxis">軸の計算先</param>
	/// <param name="covarianceMatrix"></param>
	void CalcCapsuleAxisFromCovarianceMatrix(
		Vector3& newAxis,
		float covarianceMatrix[3][3],
		const Vector3& centerPos,
		const std::vector<Vector3>& posArray
	);
	/// <summary>
	/// 座標の配列から共分散行列を計算する。
	/// </summary>
	/// <param name="covarianceMatrix">共分散行列の計算先</param>
	/// <param name="leafNodeArray">座標の配列</param>
	/// <param name="centerPos">座標群の中心座標</param>
	void CalcCovarianceMatrixFromPositionList(
		float covarianceMatrix[3][3],
		const std::vector<Vector3>& posArray,
		const Vector3& centerPos
	);
	/// <summary>
	/// 座標群の中心座標を計算する。
	/// </summary>
	/// <param name="posArray"></param>
	/// <returns></returns>
	Vector3 CalcCenterPosition(
		const std::vector<Vector3>& posArray
	);
public:
	/// <summary>
	/// 座標群から生成される代表軸を計算する
	/// </summary>
	/// <param name="leafNodeArray"></param>
	Vector3 CalcAxis(const std::vector<Vector3>& posArray);
};

