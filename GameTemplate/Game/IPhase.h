#pragma once

namespace Game
{
	class IPhase
	{
	public:

		//フェーズの状態
		enum PhaseStatus
		{
			BeforeStart,	//スタート前
			StartFrame,		//スタートフレーム
			AfterStart,		//スタート後
		};

		/**
		 * @brief フェーズの初期化処理
		*/
		virtual void Init() {}

		/**
		 * @brief フェーズの開始時処理
		*/
		virtual void PhaseStart() {}

		bool Update()
		{
			//フェーズの状態で分岐
			switch(GetPhaseStatus())
			{
			case BeforeStart://スタート前
				return false;
				break;
			case StartFrame://スタートフレーム
				PhaseStart();
				return false;
				break;
			case AfterStart://スタート後
				return PhaseUpdate();
			}

			return false;
		}

		/**
		 * @brief フェーズの更新処理
		 * @return フェーズが終了した?
		*/
		virtual bool PhaseUpdate() = 0;

		/**
		 * @brief フェーズがスタートするフレームをセットする
		 * @param phaseStartFrame フェーズがスタートするフレーム 
		*/
		void SetPhaseStartFrame(int phaseStartFrame)
		{
			m_phaseStartFrame = phaseStartFrame;
		}

		/**
		 * @brief フェーズのフレーム数を増加させる
		*/
		void PhaseTimeProgress()
		{
			m_phaseWaitFrame++;
		}

		/**
		 * @brief フェーズのフレーム数を増加させ、フェーズの状態を取得する
		 * @return フェーズの状態
		*/
		PhaseStatus GetPhaseStatus()
		{
			//フェーズのフレーム数を増加させる
			PhaseTimeProgress();

			//スタートフレームと同じなら
			if (m_phaseWaitFrame == m_phaseStartFrame)
			{
				//スタートフレーム
				return StartFrame;
			}
			//スタートフレーム以降なら
			else if (m_phaseWaitFrame > m_phaseStartFrame)
			{
				//スタート後
				return AfterStart;
			}

			//スタート前
			return BeforeStart;
		}

		/**
		 * @brief 経過したフレーム数を取得
		 * @return 経過したフレーム数
		*/
		int GetPhaseWaitFrame()
		{
			return m_phaseWaitFrame;
		}

	private:
		int m_phaseWaitFrame = 0;	//フェーズがスタートするまでに待機したフレーム数
		int m_phaseStartFrame = 0;	//フェーズがスタートするフレーム
	};
}