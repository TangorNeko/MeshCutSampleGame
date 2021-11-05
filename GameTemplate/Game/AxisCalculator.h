#pragma once
class AxisCalculator
{
private:
	/// <summary>
	/// �����U�s�񂩂玲���v�Z����B
	/// </summary>
	/// <param name="newAxis">���̌v�Z��</param>
	/// <param name="covarianceMatrix"></param>
	void CalcCapsuleAxisFromCovarianceMatrix(
		Vector3& newAxis,
		float covarianceMatrix[3][3],
		const Vector3& centerPos,
		const std::vector<Vector3>& posArray
	);
	/// <summary>
	/// ���W�̔z�񂩂狤���U�s����v�Z����B
	/// </summary>
	/// <param name="covarianceMatrix">�����U�s��̌v�Z��</param>
	/// <param name="leafNodeArray">���W�̔z��</param>
	/// <param name="centerPos">���W�Q�̒��S���W</param>
	void CalcCovarianceMatrixFromPositionList(
		float covarianceMatrix[3][3],
		const std::vector<Vector3>& posArray,
		const Vector3& centerPos
	);
	/// <summary>
	/// ���W�Q�̒��S���W���v�Z����B
	/// </summary>
	/// <param name="posArray"></param>
	/// <returns></returns>
	Vector3 CalcCenterPosition(
		const std::vector<Vector3>& posArray
	);
public:
	/// <summary>
	/// ���W�Q���琶��������\�����v�Z����
	/// </summary>
	/// <param name="leafNodeArray"></param>
	Vector3 CalcAxis(const std::vector<Vector3>& posArray);
};

