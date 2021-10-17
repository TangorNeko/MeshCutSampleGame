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

	void CutSurfaceMaker::CalcIn2D(const Vector3& normal)
	{
		//�ϒ��z���0�Ԗڂ̍��W����1�Ԗڂ̍��W�܂ł̃x�N�g����x���Ƃ������W�n���쐬����
		Vector3 ex = m_vectorContainer[1] - m_vectorContainer[0];
		ex.Normalize();

		//z���͐ؒf���ʂ̖@���Ƃ���
		Vector3 ez = normal;
		ez.Normalize();

		//y����x����z���̊O�ςŋ��߂�
		Vector3 ey = ex;
		ey.Cross(ez);
		ey.Normalize();
		
		//��ꎲ�ϊ��̍s����쐬
		Matrix transitionMatrix = g_matIdentity;
		transitionMatrix.v[0] = { ex.x,ex.y,ex.z,0 };
		transitionMatrix.v[1] = { ey.x,ey.y,ey.z,0 };
		transitionMatrix.v[2] = { ez.x,ez.y,ez.z,0 };
		transitionMatrix.Inverse();

		//3D�ł̕��ʏ�̍��W��ؒf�ʂ̌����̕������猩��2D��Ԃɕϊ�����
		for (auto& pos : m_vectorContainer)
		{
			//m_vectorContainer��0�Ԗڂ�2D��ԏ�̌��_�Ƃ���
			Vector3 calcVec = pos - m_vectorContainer[0];
			transitionMatrix.Apply3x3(calcVec);

			//���̕ϊ��ɂ����3D��Z���W�͑S�ē����ɂȂ�̂�Z����������2D��ԏ�̍��W�ɂȂ�
 			Vector2 converted2DVec = { calcVec.x,calcVec.y };

			//�ϒ��z��Ɋi�[
			m_2DArray.push_back(converted2DVec);
		}
	}
}