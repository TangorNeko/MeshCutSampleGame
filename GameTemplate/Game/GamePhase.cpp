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
		//すべてのフェーズのポインタを削除
		for (auto phase : m_phaseVector)
		{
			delete phase;
		}
	}

	void GamePhase::Init()
	{
		//フェーズを追加
		m_phaseVector.push_back(new Phase0);
		m_phaseVector.push_back(new Phase1);
		m_phaseVector.push_back(new Phase2);
		m_phaseVector.push_back(new Phase3);
		m_phaseVector.push_back(new Phase4);
		m_phaseVector.push_back(new Phase5);

		//最初のフェーズを現在のフェーズに格納
		m_currentPhase = m_phaseVector.front();

		//フェーズの初期化処理
		m_currentPhase->Init();
	}

	void GamePhase::Update()
	{
		//フェーズの処理を実行し、フェーズが終了したかを取得
		bool isFinish = m_currentPhase->Update();

		//フェーズが終了していたら
		if (isFinish)
		{
			//フェーズ進行
			Progress();
		}
	}

	void GamePhase::Progress()
	{
		//フェーズ番号を増加
		m_currentPhaseNo++;

		//番号に応じたフェーズを格納
		m_currentPhase = m_phaseVector.at(m_currentPhaseNo);

		//フェーズの初期化処理を行う
		m_currentPhase->Init();
	}

	void GamePhase::SkipTo(int phaseNo)
	{
		//指定したフェーズが現在のフェーズより前のフェーズなら何もしない
		if (phaseNo <= m_currentPhaseNo)
		{
			return;
		}

		//指定したフェーズ番号をセット
		m_currentPhaseNo = phaseNo;

		//番号に応じたフェーズを格納
		m_currentPhase = m_phaseVector.at(m_currentPhaseNo);
		
		//フェーズの初期化処理を行う
		m_currentPhase->Init();
	}
}
