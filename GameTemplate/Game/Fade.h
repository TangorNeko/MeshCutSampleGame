#pragma once

namespace Game
{
	class Fade : public IGameObject
	{
	public:
		~Fade();

		bool Start() override;

		void Update() override;

		void FadeIn();

		void Wait();

		void FadeOut();

		void SetFadeInRate(float rate)
		{
			m_fadeInRate = rate;
		}
		
		void SetWaitFrame(int frame)
		{
			m_waitFrame = frame;
		}
		
		void SetFadeOutRate(float rate)
		{
			m_fadeOutRate = rate;
		}

		float GetFadeProgress()
		{
			return m_mulColor.w;
		}
	private:
		SpriteRender* m_fadeSprite = nullptr;

		Vector4 m_mulColor = { 0.0f,0.0f,0.0f,0.0f };

		float m_fadeInRate = 0.01f;

		int m_waitFrame = 0;

		int m_currentWaitFrame = 0;
		
		float m_fadeOutRate = 0.01f;

		enum FadeState
		{
			enFadeIn,
			enWait,
			enFadeOut
		};

		FadeState m_fadeState = enFadeIn;
	};
}
