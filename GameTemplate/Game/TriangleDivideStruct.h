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
		const Vector3& GetNormal()const { return planeNormal; }

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
		uint32_t vertexIndexes[3];					//���_�̃C���f�b�N�X
	};

	/**
	 * @brief ���_�̃f�[�^
	*/
	struct VertexIndexesPack
	{
		std::vector<uint32_t> frontVertexIndexes;	//���ʂ̕\���ɂ��钸�_�̃C���f�b�N�X�̔z��
		std::vector<uint32_t> backVertexIndexes;		//���ʂ̗����ɂ��钸�_�̃C���f�b�N�X�z��
		std::vector<uint32_t> onPlaneVertexIndexes;	//���ʏ�ɂ��钸�_�̃C���f�b�N�X�̔z��
	};
}
