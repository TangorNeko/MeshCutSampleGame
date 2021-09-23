#pragma once

namespace Util
{
	/**
	 * @brief ���ʂ̃f�[�^
	*/
	struct PlaneData
	{
		Vector3 planePoint = Vector3::Zero;		//���ʂ̈�_
		/**
		 * @brief �@����ݒ肵���K��
		 * @param normal �@���̃x�N�g��
		*/
		void SetNormal(const Vector3& normal)
		{
			if (normal.LengthSq() == 0)
			{
				MessageBoxA(nullptr, "�s���Ȗ@�����Z�b�g���Ă��܂��B\n", "�G���[", MB_OK);
				std::abort();
			}
			planeNormal = normal;
			planeNormal.Normalize();
		}
		/**
		 * @brief �@�����擾
		 * @return �@���̃x�N�g��
		*/
		const Vector3& GetNormal()const  { return planeNormal; }
	private:
		Vector3 planeNormal = Vector3::Right;	//���ʂ̖@��
	};

	/**
	 * @brief �O�p�`�̃f�[�^
	*/
	struct TriangleData
	{
		Vector3 vertices[3];					//���_
	};

	/**
	 * @brief ���_�̃f�[�^
	*/
	struct VerticesPack
	{
		std::vector<Vector3> frontVertices;		//���ʂ̕\���ɂ��钸�_�̔z��
		std::vector<Vector3> backVertices;		//���ʂ̗����ɂ��钸�_�̔z��
		std::vector<Vector3> onPlaneVertices;	//���ʏ�ɂ��钸�_�̔z��
	};


	/**
	 * @brief ���ʂ��O�p�`�𕪊����Ă���?
	 * @param[in] planeData ���ʂ̃f�[�^
	 * @param[in] triangleData �O�p�`�̃f�[�^
	 * @param[out] verticesPack ���_�̃f�[�^
	 * @return �������Ă���:true ���Ă��Ȃ�:false
	*/
	bool IsPlaneDivideTriangle(const PlaneData& planeData, const TriangleData& triangleData, VerticesPack& verticesPack);

	/**
	 * @brief ���ʂƐ����Ƃ̌����n�_�����߂�(�q�b�g���Ă���O��)
	 * @param planeData[in] ���ʂ̃f�[�^
	 * @param startPoint[in] �����̊J�n�ʒu
	 * @param endPoint[in] �����̏I���ʒu
	 * @return �����n�_
	*/
	Vector3 GetCrossPoint(const PlaneData& planeData, const Vector3& startPoint, const Vector3& endPoint);

	/**
	 * @brief ���ʂƃO���[�v�������ꂽ���_���番�����ꂽ�����̒��_�����߂�
	 * @param planeData[in] ���ʂ̃f�[�^
	 * @param verticesPack[in] ���_�̃f�[�^(�O���[�v������)
	 * @param points[out] ���������̒��_�̔z��(�v�f��2) 
	 * @return �������ꂽ�ېV�����ł������_�̐�
	*/
	int GetDividedPoint(const PlaneData& planeData, const VerticesPack& verticesPack, std::array<Vector3, 2>& points);
}