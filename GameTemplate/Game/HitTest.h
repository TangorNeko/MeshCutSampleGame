#pragma once

namespace Util
{
	/**
	 * @brief ���ʂ̃f�[�^
	*/
	struct PlaneData
	{
		/**
		 * @brief ���ʏ�̈�_���擾
		 * @return ���ʏ�̈�_�̍��W
		*/
		const Vector3& GetPoint() const { return planePoint; }

		/**
		 * @brief ���ʏ�̈�_��ݒ�
		 * @param point ���ʏ�̈�_�̍��W
		*/
		void SetPoint(const Vector3& point)
		{
			planePoint = point;
		}

		/**
		 * @brief �@�����擾
		 * @return �@���̃x�N�g��
		*/
		const Vector3& GetNormal()const  { return planeNormal; }

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
	private:
		Vector3 planePoint = Vector3::Zero;		//���ʂ̈�_
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
	struct VertexIndexesPack
	{
		std::vector<int> frontVertexIndexes;	//���ʂ̕\���ɂ��钸�_�̃C���f�b�N�X�̔z��
		std::vector<int> backVertexIndexes;		//���ʂ̗����ɂ��钸�_�̃C���f�b�N�X�z��
		std::vector<int> onPlaneVertexIndexes;	//���ʏ�ɂ��钸�_�̃C���f�b�N�X�̔z��
	};

	class TriangleDivider
	{
	public:

		/**
		 * @brief �������ʂ̃Z�b�g
		 * @param planeData �������ʂ̃f�[�^
		*/
		void SetPlane(const PlaneData& planeData)
		{
			m_planeData = planeData;
			m_isPlaneDataRegisterd = true;
		}

		/**
		 * @brief �����̎��s(��)�@�K���������ʂ��Z�b�g���Ă���ĂԎ�
		 * @param triangleData[in] ���������s����O�p�`�̃f�[�^
		*/
		void Divide(const TriangleData& TriangleData);

		//private:

		/**
		 * @brief �f�[�^�̃��Z�b�g
		*/
		void DataReset()
		{
			//�������_���擾�������̃t���O�����Z�b�g
			m_alreadyGetAnyDividePoint = false;

			//�ʂ̗��E�\�E��ɑ��݂��钸�_�̃C���f�b�N�X�f�[�^�����Z�b�g
			m_vertexIndexesPack.frontVertexIndexes.clear();
			m_vertexIndexesPack.backVertexIndexes.clear();
			m_vertexIndexesPack.onPlaneVertexIndexes.clear();
		}

		/**
		 * @brief ���ʂ��O�p�`�𕪊����Ă���?
		 * @return �������Ă���:true ���Ă��Ȃ�:false
		*/
		bool IsPlaneDivideTriangle();

		/**
		 * @brief ���ʂƐ����Ƃ̌����n�_�����߂�(�q�b�g���Ă���O��)
		 * @param startPoint[in] �����̊J�n�ʒu
		 * @param endPoint[in] �����̏I���ʒu
		 * @return �����n�_
		*/
		Vector3 GetCrossPoint(const Vector3& startPoint, const Vector3& endPoint);

		/**
		 * @brief 0,1,2�̃C���f�b�N�X�ԍ��̂����܂܂�Ă��Ȃ�����Ԃ�
		 * @param first 1�ڂ̃C���f�b�N�X�̐���
		 * @param second 2�ڂ̃C���f�b�N�X�̐���
		 * @return �Ō�Ɏc�����C���f�b�N�X�̐���
		*/
		int GetLeftoverOfThree(int first, int second)
		{
			return 3 - first - second;
		}

		/**
		 * @brief ���ʂƃO���[�v�������ꂽ���_���番�����ꂽ�����̒��_�����߂�
		 * @param verticesPack[in] ���_�̃f�[�^(�O���[�v������)
		 * @param points[out] ���������̒��_�̔z��(�v�f��2)
		 * @return �������ꂽ�ېV�����ł������_�̐�
		*/
		int GetDividedPoint(std::array<Vector3, 2>& points);

	private:
		bool m_isPlaneDataRegisterd = false;		//�������ʃf�[�^���o�^����Ă���?
		bool m_alreadyGetAnyDividePoint = false;	//���łɕ������_��1�ł��擾����?
		PlaneData m_planeData;						//�������ʃf�[�^
		TriangleData m_triangleData;				//���������O�p�`�̃f�[�^
		VertexIndexesPack m_vertexIndexesPack;		//�ʂ̗��E�\�E��ɑ��݂��钸�_�̃C���f�b�N�X�f�[�^
	};
}