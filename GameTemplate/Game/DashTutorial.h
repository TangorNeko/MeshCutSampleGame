#pragma once

namespace Game
{
	class DashTutorial : public IGameObject
	{
		~DashTutorial();

		bool Start() override;

		void Update() override;

	private:
		SpriteRender* m_spriteRender = nullptr; //チュートリアルのスプライト
		int m_popUpProgress = 0;				//ポップアップ処理の進度
	};
}