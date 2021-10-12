#include "stdafx.h"
#include "CutSurfaceMaker.h"

namespace Util
{
	void CutSurfaceMaker::MakeLinkFromSet()
	{
		int linkIndex = 0;
		m_pointLinkArray.resize(1);
		while (true)
		{
			for (auto it = m_segmentSet->begin();it != m_segmentSet->end();)
			{
				if (m_pointLinkArray[linkIndex].size() == 0)
				{
					m_pointLinkArray[linkIndex].push_back((*it).first);
					m_pointLinkArray[linkIndex].push_back((*it).second);

					it = m_segmentSet->erase(it);
				}
				else
				{
					int end = *(m_pointLinkArray[linkIndex].end() - 1);
					if ((*it).first == end)
					{
						m_pointLinkArray[linkIndex].push_back((*it).second);
						m_segmentSet->erase(it);
						it = m_segmentSet->begin();
					}
					else if ((*it).second == end)
					{
						m_pointLinkArray[linkIndex].push_back((*it).first);
						m_segmentSet->erase(it);
						it = m_segmentSet->begin();
					}
					else
					{
						it++;
					}
				}
			}


			if (m_segmentSet->size() == 0)
			{
				break;
			}
			linkIndex++;
			m_pointLinkArray.resize(linkIndex + 1);
		}

		int a = 5;
	}
}