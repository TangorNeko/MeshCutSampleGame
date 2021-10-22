#pragma once
#include "tkFile/TkmFile.h"

namespace Util
{
	class CutSurfaceMaker
	{
	public:
		/**
		 * @brief ������
		 * @param segmentSet �����̃C���f�b�N�X�̏W��
		 * @param vertexContainer ���f���̒��_�o�b�t�@
		*/
		void Init(std::set<std::pair<uint16_t, uint16_t>>* segmentSet, std::vector<TkmFile::SVertex>* vertexContainer)
		{
			m_segmentSet = segmentSet;
			m_vertexContainer = vertexContainer;
		}

		/**
		 * @brief �����̃C���f�b�N�X�̏W��������W���擾���ĐV�����W�����쐬����
		*/
		void ConvertFromSet();

		/**
		 * @brief �V�����W�����璸�_�Ԃ̃����N���쐬����
		*/
		void MakeLinkFromSet();

		/**
		 * @brief �ؒf�ʂ̒��_��2D��Ԃɕϊ�����
		 * @param normal �ؒf�ʂ̖@��
		 * @return �ؒf�ʂ̐����̕K�v����:true �����̕K�v�Ȃ�:false
		*/
		bool CalcIn2D(const Vector3& normal);

		/**
		 * @brief ���̃����N�͎��v��肩?
		 * @param pointLink �ؒf�ʂ�1�̃����N
		 * @return ���v���:true �����v���:false
		*/
		bool IsClockwise(const std::vector<uint16_t> pointLink);

		/**
		 * @brief �ؒf�ʂ̐���
		 * @param vertexBuffer ���f���̒��_�o�b�t�@
		 * @param frontIndexBufferArray �\���̃C���f�b�N�X�o�b�t�@�̔z��
		 * @param backIndexBufferArray �����̃C���f�b�N�X�o�b�t�@�̔z��
		*/
		void MakeSurface(std::vector<TkmFile::SVertex>* vertexBuffer,std::vector<TkmFile::SIndexbuffer16>* frontIndexBufferArray, std::vector<TkmFile::SIndexbuffer16>* backIndexBufferArray);
	private:
		std::set<std::pair<uint16_t, uint16_t>>* m_segmentSet = nullptr;	//�ؒf�ʏ�ɂ�������̃C���f�b�N�X�̑g�ݍ��킹�̏W��
		std::vector<TkmFile::SVertex>* m_vertexContainer = nullptr;			//���f���̒��_�o�b�t�@
		std::vector<Vector3> m_vectorContainer;								//�ؒf�ʂ̒��_�̉ϒ��z��(�d���Ȃ�)
		std::map<Vector3, int> m_vectorMap;									//Vector3�Ɛؒf�ʂ̒��_�̉ϒ��z����̈ʒu���i�[����A�z�z��
		std::set<std::pair<int, int>> m_convertedSet;						//m_segmentSet��m_vectorContainer��̃C���f�b�N�X�ɕϊ����Ċi�[��������
		std::vector<std::vector<uint16_t>> m_pointLinkArray;				//�ؒf�ʂ��\�����郊���N�̔z��
		std::vector<Vector2> m_2DArray;										//�ؒf�ʏ�̒��_��2D��Ԃł̍��W(m_vectorContainer�ƑΉ�)
		std::map<Vector2, int> m_2DMap;										//�ؒf�ʏ�̒��_��2D��Ԃł̍��W��m_pointLinkArray�̉��Ԗڂɂ��邩�̘A�z�z��
		Vector3 m_ex = Vector3::Zero;										//�ؒf�ʍ��W�n�̊�ꎲ
		Vector3 m_ey = Vector3::Zero;
		Vector3 m_ez = Vector3::Zero;
	};
}