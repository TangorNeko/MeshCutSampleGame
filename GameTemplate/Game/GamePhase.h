#pragma once

namespace Game
{
	class IPhase;

	class GamePhase
	{
	public:
		~GamePhase();

		/**
		 * @brief 各フェーズの初期化
		*/
		void Init();

		/**
		 * @brief フェーズのアップデート
		*/
		void Update();

		/**
		 * @brief 次のフェーズに進行させる
		*/
		void Progress();

		/**
		 * @brief 指定した番号のフェーズにスキップさせる
		 * @param phaseNo フェーズ番号
		*/
		void SkipTo(int phaseNo);
	
	private:
		std::vector<IPhase*> m_phaseVector;	//フェーズの配列
		IPhase* m_currentPhase = nullptr;	//現在のフェーズ
		int m_currentPhaseNo = 0;			//現在のフェーズ番号
	};
}

