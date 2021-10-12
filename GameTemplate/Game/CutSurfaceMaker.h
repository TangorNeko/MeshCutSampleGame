#pragma once
namespace Util
{
	class CutSurfaceMaker
	{
	public:
		void SetSegmentSet(std::set<std::pair<uint16_t, uint16_t>>* segmentSet)
		{
			m_segmentSet = segmentSet;
		}

		void MakeLinkFromSet();
	private:
		std::set<std::pair<uint16_t, uint16_t>>* m_segmentSet = nullptr;		//�ؒf�ʏ�ɂ�������̃C���f�b�N�X�̑g�ݍ��킹�̏W��
		std::vector<std::vector<uint16_t>> m_pointLinkArray;	//�ؒf�ʂ��\�����郊���N�̔z��
	};
}