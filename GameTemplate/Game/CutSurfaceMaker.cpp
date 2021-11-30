#include "stdafx.h"
#include "CutSurfaceMaker.h"
#include "TwoDTriangulate.h"
#include <algorithm>

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

	bool CutSurfaceMaker::CalcIn2D(const Vector3& normal)
	{
		//�ؒf�ʂ̒��_����3�����Ȃ�ؒf�ʂ𐶐�����K�v���Ȃ��B
		if (m_vectorContainer.size() < 3)
		{
			return false;
		}
		//�ϒ��z���0�Ԗڂ̍��W����1�Ԗڂ̍��W�܂ł̃x�N�g����x���Ƃ������W�n���쐬����
		m_ex = m_vectorContainer[1] - m_vectorContainer[0];
		m_ex.Normalize();

		//z���͐ؒf���ʂ̖@���Ƃ���
		m_ez = normal;
		m_ez.Normalize();

		//y����x����z���̊O�ςŋ��߂�
		m_ey = m_ex;
		m_ey.Cross(m_ez);
		m_ey.Normalize();
		
		//��ꎲ�ϊ��̍s����쐬
		Matrix transitionMatrix = g_matIdentity;
		transitionMatrix.v[0] = { m_ex.x,m_ex.y,m_ex.z,0 };
		transitionMatrix.v[1] = { m_ey.x,m_ey.y,m_ey.z,0 };
		transitionMatrix.v[2] = { m_ez.x,m_ez.y,m_ez.z,0 };
		transitionMatrix.Inverse();

		//3D�ł̕��ʏ�̍��W��ؒf�ʂ̌����̕������猩��2D��Ԃɕϊ�����

		//�A�z�z��ɓo�^����p�̉��Ԗڂ��̃C���f�b�N�X�ԍ�
		int index = 0;
		for (auto& pos : m_vectorContainer)
		{
			//m_vectorContainer��0�Ԗڂ�2D��ԏ�̌��_�Ƃ���
			Vector3 calcVec = pos - m_vectorContainer[0];

			//��ꎲ�ϊ�
			transitionMatrix.Apply3x3(calcVec);

			//�ϊ��ɂ����3D��Z���W�͑S�ē����ɂȂ�̂�Z����������2D��ԏ�̍��W�ɂȂ�
 			Vector2 converted2DVec = { calcVec.x,calcVec.y };

			//�ϒ��z��Ɋi�[
			m_2DArray.push_back(converted2DVec);

			//���p�`�����Ɏg�p����A�z�z��ɂ��C���f�b�N�X�Ƌ��Ɋi�[
			m_2DMap.insert(std::make_pair(converted2DVec,index));

			//�C���f�b�N�X�𑝂₷
			index++;
		}

		return true;
	}

	bool CutSurfaceMaker::IsClockwise(const std::vector<uint16_t> pointLink)
	{
		Vector2 minPoint = { FLT_MAX,FLT_MAX };
		std::vector<uint16_t>::const_iterator minIt;

		for (auto it = pointLink.begin(); it != pointLink.end(); it++)
		{
			//���X��������(X�������Ȃ�Y��������)�Ȃ�
			if (m_2DArray[*it].x <= minPoint.x && m_2DArray[*it].y <= minPoint.y)
			{
				//���W���擾
				minPoint = m_2DArray[*it];

				//��ԏ������n�_�̑O��𒲂ׂ邽�߁A�C�e���[�^�[���擾
				minIt = it;
			}
		}

		//�ŏ��l�_�̑O����擾
		uint16_t prev, next;
		if (minIt == pointLink.begin())
		{
			//�ŏ��n�_���ŏ��̗v�f�������ꍇ�A��O�̗v�f�͍Ō�̗v�f�Ƃ���
			prev = *(pointLink.end() - 1);
			next = *(minIt + 1);
		}
		else if (minIt == pointLink.end() - 1)
		{
			prev = *(minIt - 1);
			//�ŏ��n�_���Ō�̗v�f�������ꍇ�A��O�̗v�f�͍ŏ��̗v�f�Ƃ���
			next = *(pointLink.begin());
		}
		else
		{
			prev = *(minIt - 1);
			next = *(minIt + 1);
		}

		Vector2 prevPoint = m_2DArray[prev];
		Vector2 nextPoint = m_2DArray[next];
		
		//prevPoint����minPoint�Ɉ����������AnextPoint���E�ɂ���Ύ��v���A���ɂ���Δ����v���
		Vector2 prevToMin, prevToNext;
		prevToMin.Subtract(minPoint, prevPoint);
		prevToNext.Subtract(nextPoint,prevPoint);

		if (Cross(prevToMin, prevToNext) > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void CutSurfaceMaker::MakeSurface(std::vector<TkmFile::SVertex>& vertexBuffer, std::vector<TkmFile::SVertex>& backVertexBuffer,std::vector<TkmFile::SIndexbuffer16>* frontIndexBufferArray, std::vector<TkmFile::SIndexbuffer16>* backIndexBufferArray)
	{

		//�i�[�������W���瑽�p�`����
		TwoDTriangulate triangulator;
		//�A�z�z����Z�b�g
		triangulator.SetMap(&m_2DMap);

		//�V�����ł���O�p�`�̃C���f�b�N�X�������N���ƂɊi�[����ϒ��z��
		std::vector<std::vector<int>> cutPlaneNewIndexBufferArray;

		//���p�`�������s�����_�Q
		std::vector<Vector2> triangulateVertexes;

		//�e�����N�𑖍�
		for (auto& pointLink : m_pointLinkArray)
		{
			//�����N���m������Ă��邩�̔���
			if (*(pointLink.begin()) == *(pointLink.end() - 1))
			{
				pointLink.erase(pointLink.end() - 1);
			}
			else
			{
				//TODO:return�łȂ��������s��Ȃ��ݒ�?
				//�����N���m������Ă��Ȃ��Ă��f�ʂ���肽�����͂�����
				continue;
			}


			//�����N��2D���W��Ŏ��v��肩�����v���肩�̔���
			//���p�`���������邽�߂ɂ̓����N�������v����łȂ��Ƃ����Ȃ�����
			//���v���Ȃ�
			if (IsClockwise(pointLink) == true)
			{
				//���]
				std::reverse(pointLink.begin(), pointLink.end());
			}

			//�����N�̏��Ԃɏ]���č��W���i�[
			for (auto pointIndex : pointLink)
			{
				triangulateVertexes.push_back(m_2DArray[pointIndex]);
			}
			//����
			triangulator.Triangulate(triangulateVertexes);

			//1�̃����N���̃C���f�b�N�X�o�b�t�@���i�[
			cutPlaneNewIndexBufferArray.push_back(triangulator.GetBuffer());

			//���_�Q�����Z�b�g
			triangulateVertexes.clear();
		}

		//�V�����쐬���闠���p�̋�̒��_�o�b�t�@�Ɍ��X�̃��f���̒��_�o�b�t�@���R�s�[���Ă���
		backVertexBuffer = vertexBuffer;

		//���_�o�b�t�@�̍Ō�̈ʒu(�ǉ������ꍇ�̃C���f�b�N�X)���擾
		int startIndex = vertexBuffer.size();
		//�������璸�_�̐���
		int uvCount = 0;
		for (auto pos : m_vectorContainer)
		{
			TkmFile::SVertex addVert;
			addVert.pos = pos;


			//TODO:UV�͉�
			addVert.uv = Vector2::Zero;
			uvCount++;

			//�X�L���֌W?�悭������Ȃ�
			addVert.indices[0] = 0;
			addVert.indices[1] = 0;
			addVert.indices[2] = 0;
			addVert.indices[3] = 0;
			addVert.skinWeights = { 0.0f,0.0f,0.0f,0.0f };

			//NOTE:�ؒf�ʍ��W�n���g�p
			//�ؒf�ʍ��W�n��Z���͕\�������Ȃ̂ŁA�����̐ؒf�ʂ̖@���͂��̂܂܎g�p����B
			addVert.normal = m_ez;
			addVert.tangent = m_ex;
			addVert.binormal = m_ey;

			//�����̃��f���p�̒��_�o�b�t�@�Ɋi�[
			backVertexBuffer.push_back(addVert);

			//�\���̐ؒf�ʂ̖@�����\���������Ƌt�ɂȂ�̂Ŕ��]����B
			addVert.normal *= -1;
			addVert.tangent *= -1;
			addVert.binormal *= -1;

			//�\���̃��f���p�̒��_�o�b�t�@�Ɋi�[
			vertexBuffer.push_back(addVert);
		}

		//�����N���Ƃɕ�����ꂽ�C���f�b�N�X�o�b�t�@�̔z��𑖍�����
		for (const auto& cutPlaneNewIndexBuffer : cutPlaneNewIndexBufferArray)
		{
			for (auto newIndex : cutPlaneNewIndexBuffer)
			{
				//NOTE:����0�Ԗڂ̃}�e���A���̃C���f�b�N�X�Ƃ��Ēǉ�
				//�����V�����}�e���A����K�p����Ƃ����Material�𑝂₷�K�v������?
				frontIndexBufferArray->at(0).indices.push_back(newIndex + startIndex);
				backIndexBufferArray->at(0).indices.push_back(newIndex + startIndex);
			}
		}
	}
}