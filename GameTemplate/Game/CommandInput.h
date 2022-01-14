#pragma once

namespace Game
{
	class CommandInput : public IGameObject
	{
	public:
		~CommandInput();

		bool Start() override;

		void Update() override;

		void FadeOut();

		bool isSuccess()
		{
			return m_isSuccess;
		}
	private:
		SpriteRender* m_commandSprite = nullptr;
		bool m_isSuccess = false;
		int m_fadeFrame = 0;
	};
}

