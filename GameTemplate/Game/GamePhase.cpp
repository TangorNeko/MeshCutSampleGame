#include "stdafx.h"
#include "GamePhase.h"
#include "Phase0.h"
#include "Phase1.h"
#include "Phase2.h"
#include "Phase3.h"
#include "Phase4.h"
#include "Phase5.h"

namespace Game
{
	GamePhase::~GamePhase()
	{
		//���ׂẴt�F�[�Y�̃|�C���^���폜
		for (auto phase : m_phaseVector)
		{
			delete phase;
		}
	}

	void GamePhase::Init()
	{
		//�t�F�[�Y��ǉ�
		m_phaseVector.push_back(new Phase0);
		m_phaseVector.push_back(new Phase1);
		m_phaseVector.push_back(new Phase2);
		m_phaseVector.push_back(new Phase3);
		m_phaseVector.push_back(new Phase4);
		m_phaseVector.push_back(new Phase5);

		//�ŏ��̃t�F�[�Y�����݂̃t�F�[�Y�Ɋi�[
		m_currentPhase = m_phaseVector.front();

		//�t�F�[�Y�̏���������
		m_currentPhase->Init();
	}

	void GamePhase::Update()
	{
		//�t�F�[�Y�̏��������s���A�t�F�[�Y���I�����������擾
		bool isFinish = m_currentPhase->Update();

		//�t�F�[�Y���I�����Ă�����
		if (isFinish)
		{
			//�t�F�[�Y�i�s
			Progress();
		}
	}

	void GamePhase::Progress()
	{
		//�t�F�[�Y�ԍ��𑝉�
		m_currentPhaseNo++;

		//�ԍ��ɉ������t�F�[�Y���i�[
		m_currentPhase = m_phaseVector.at(m_currentPhaseNo);

		//�t�F�[�Y�̏������������s��
		m_currentPhase->Init();
	}

	void GamePhase::SkipTo(int phaseNo)
	{
		//�w�肵���t�F�[�Y�����݂̃t�F�[�Y���O�̃t�F�[�Y�Ȃ牽�����Ȃ�
		if (phaseNo <= m_currentPhaseNo)
		{
			return;
		}

		//�w�肵���t�F�[�Y�ԍ����Z�b�g
		m_currentPhaseNo = phaseNo;

		//�ԍ��ɉ������t�F�[�Y���i�[
		m_currentPhase = m_phaseVector.at(m_currentPhaseNo);
		
		//�t�F�[�Y�̏������������s��
		m_currentPhase->Init();
	}
}
