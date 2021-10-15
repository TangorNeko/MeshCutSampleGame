#include "stdafx.h"
#include "CutSurfaceMaker.h"

namespace Util
{
	void CutSurfaceMaker::ConvertFromSet()
	{
		int setVal1, setVal2;

		//�����̃C���f�b�N�X�̏W���𑖍�
		for (auto it = m_segmentSet->begin(); it != m_segmentSet->end();it++)
		{
			//�����̎n�_�̃C���f�b�N�X������W���擾
			Vector3 key = m_vertexContainer->at((*it).first).pos;

			//���W���L�[�Ƃ��ĘA�z�z�񂩂�C���f�b�N�X������
			auto foundIndex = m_vectorMap.find(key);
			if (foundIndex != m_vectorMap.end())
			{
				//����������A�C���f�b�N�X���i�[
				setVal1 = (*foundIndex).second;
			}
			else
			{
				//������Ȃ�������A���W��V�K�ǉ����Ă��̃C���f�b�N�X���i�[
				setVal1 = m_vectorContainer.size();
				m_vectorContainer.push_back(key);
				m_vectorMap.insert(std::make_pair(key, setVal1));
			}

			//TODO:first��second���Ⴄ�����ŌJ��Ԃ��A��������?

			//�����̏I�_�̃C���f�b�N�X������W���擾
			key = m_vertexContainer->at((*it).second).pos;

			//���W���L�[�Ƃ��ĘA�z�z�񂩂�C���f�b�N�X������
			foundIndex = m_vectorMap.find(key);
			if (foundIndex != m_vectorMap.end())
			{
				//����������A�C���f�b�N�X���i�[
				setVal2 = (*foundIndex).second;
			}
			else
			{
				//������Ȃ�������A���W��V�K�ǉ����Ă��̃C���f�b�N�X���i�[
				setVal2 = m_vectorContainer.size();
				m_vectorContainer.push_back(key);
				m_vectorMap.insert(std::make_pair(key, setVal2));
			}

			//�\�[�g���ĕϊ���̏W���Ɋi�[
			if (setVal1 < setVal2)
			{
				m_convertedSet.insert(std::make_pair(setVal1, setVal2));
			}
			else if(setVal1 > setVal2)
			{
				m_convertedSet.insert(std::make_pair(setVal2, setVal1));
			}
		}
	}

	void CutSurfaceMaker::MakeLinkFromSet()
	{
		//�����N�ԍ�
		int linkIndex = 0;

		//�����N���i�[����ϒ��z���1�g�ڂ��J����
		m_pointLinkArray.resize(1);
		while (true)
		{
			//�ϊ���̏W���𑖍�
			for (auto it = m_convertedSet.begin();it != m_convertedSet.end();)
			{
				//�����N���i�[����ϒ��z�񂪋�Ȃ�(�V���������N�Ȃ�)
				if (m_pointLinkArray[linkIndex].size() == 0)
				{
					//1�Ԗڂ�2�Ԗڂ̒l���i�[
					m_pointLinkArray[linkIndex].push_back((*it).first);
					m_pointLinkArray[linkIndex].push_back((*it).second);

					//�i�[�����l���W������폜
					it = m_convertedSet.erase(it);
				}
				else//�����N���i�[����ϒ��z�񂪋�ł͂Ȃ�(���łɃ����N�����݂���Ȃ�)
				{
					//�����N�̍Ō���̒l���擾
					int linkTail = *(m_pointLinkArray[linkIndex].end() - 1);

					//1�Ԗڂ̒l���Ō���̒l�ƈꏏ�Ȃ�
					if ((*it).first == linkTail)
					{
						//2�Ԗڂ̒l�������N�ɒǉ�
						m_pointLinkArray[linkIndex].push_back((*it).second);

						//�i�[�����l���W������폜
						m_convertedSet.erase(it);

						//�C�e���[�^�[���ŏ��ɖ߂�
						it = m_convertedSet.begin();
					}
					else if ((*it).second == linkTail)//2�Ԗڂ̒l���Ō���̒l�ƈꏏ�Ȃ�
					{
						//1�Ԗڂ̒l�������N�ɒǉ�
						m_pointLinkArray[linkIndex].push_back((*it).first);

						//�i�[�����l���W������폜
						m_convertedSet.erase(it);

						//�C�e���[�^�[���ŏ��ɖ߂�
						it = m_convertedSet.begin();
					}
					else
					{
						//�C�e���[�^�[�𑝂₷
						it++;
					}
				}
			}

			//�W���̃T�C�Y��0�Ȃ�(���ׂẴ����N�����I������)
			if (m_convertedSet.size() == 0)
			{
				break;
			}

			//�V���ȃ����N���쐬����̂ŃC���f�b�N�X�𑝂₷
			linkIndex++;

			//�����N�p�̉ϒ��z������T�C�Y
			m_pointLinkArray.resize(linkIndex + 1);
		}
	}
}