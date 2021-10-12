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
		std::set<std::pair<uint16_t, uint16_t>>* m_segmentSet = nullptr;		//切断面上にある線分のインデックスの組み合わせの集合
		std::vector<std::vector<uint16_t>> m_pointLinkArray;	//切断面を構成するリンクの配列
	};
}