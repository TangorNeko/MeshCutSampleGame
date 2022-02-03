#pragma once

namespace Game
{
	class CommandInput : public IGameObject
	{
	public:
		~CommandInput();

		bool Start() override;

		void Update() override;

		void Pause() override;

		/**
		 * @brief コマンド表示のスプライトをフェーズアウトさせる
		*/
		void FadeOut();

		/**
		 * @brief コマンド入力が成功したか?
		 * @return コマンド入力が成功したか
		*/
		bool isSuccess()
		{
			return m_isSuccess;
		}
	private:
		SpriteRender* m_commandSprite = nullptr;	//コマンドのスプライト
		bool m_isSuccess = false;					//コマンド入力が成功した?
		int m_fadeFrame = 0;						//フェードアウトの進度
	};
}

