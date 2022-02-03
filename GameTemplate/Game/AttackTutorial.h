#pragma once

namespace Game
{
	class AttackTutorial : public IGameObject
	{
		~AttackTutorial();

		bool Start() override;

		void Update() override;

	private:
		SpriteRender* m_spriteRender = nullptr;	//チュートリアルのスプライト
		int m_popUpProgress = 0;				//ポップアップ処理の進度
	};
}

