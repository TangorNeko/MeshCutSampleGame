#pragma once

namespace Game
{
	class Fade : public IGameObject
	{
	public:
		~Fade();

		bool Start() override;

		void Update() override;

		/**
		 * @brief フェードインの処理
		*/
		void FadeIn();

		/**
		 * @brief 待機中の処理
		*/
		void Wait();

		/**
		 * @brief フェードアウトの処理
		*/
		void FadeOut();

		/**
		 * @brief フェードインのレートをセット
		 * @param rate レート
		*/
		void SetFadeInRate(float rate)
		{
			m_fadeInRate = rate;
		}
		
		/**
		 * @brief 待機フレームをセット
		 * @param frame 待機フレーム
		*/
		void SetWaitFrame(int frame)
		{
			m_waitFrame = frame;
		}
		
		/**
		 * @brief フェードアウトのレートをセット
		 * @param rate レート
		*/
		void SetFadeOutRate(float rate)
		{
			m_fadeOutRate = rate;
		}

		/**
		 * @brief フェード処理の進行度を取得
		 * @return フェード処理の進行度
		*/
		float GetFadeProgress()
		{
			return m_mulColor.w;
		}
	private:
		SpriteRender* m_fadeSprite = nullptr;			//フェードに使用するスプライト
		Vector4 m_mulColor = { 0.0f,0.0f,0.0f,0.0f };	//フェードに使用するスプライトへの乗算カラー
		float m_fadeInRate = 0.01f;						//フェードインのレート
		int m_waitFrame = 0;							//待機するフレーム
		int m_currentWaitFrame = 0;						//待機の経過フレーム
		float m_fadeOutRate = 0.01f;					//フェードアウトのレート

		//フェードの進行度
		enum FadeState
		{
			enFadeIn,	//フェードイン
			enWait,		//待機
			enFadeOut	//フェードアウト
		};

		//現在のフェードの進行度
		FadeState m_fadeState = enFadeIn;
	};
}
