#pragma once
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
	private:
		std::set<std::pair<uint16_t, uint16_t>>* m_segmentSet = nullptr;	//�ؒf�ʏ�ɂ�������̃C���f�b�N�X�̑g�ݍ��킹�̏W��
		std::vector<TkmFile::SVertex>* m_vertexContainer = nullptr;			//���f���̒��_�o�b�t�@
		std::vector<Vector3> m_vectorContainer;								//�ؒf�ʂ̒��_�̉ϒ��z��(�d���Ȃ�)
		std::map<Vector3, int> m_vectorMap;									//Vector3�Ɛؒf�ʂ̒��_�̉ϒ��z����̈ʒu���i�[����A�z�z��
		std::set<std::pair<int, int>> m_convertedSet;						//m_segmentSet��m_vectorContainer��̃C���f�b�N�X�ɕϊ����Ċi�[��������
		std::vector<std::vector<uint16_t>> m_pointLinkArray;				//�ؒf�ʂ��\�����郊���N�̔z��
	};
}